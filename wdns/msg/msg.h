#include <stdint.h>

#define WDNS_FLAGS_QR(flags)		((flags >> 15) & 0x01)
#define WDNS_FLAGS_OPCODE(flags)	((flags >> 11) & 0x0f)
#define WDNS_FLAGS_AA(flags)		((flags >> 10) & 0x01)
#define WDNS_FLAGS_TC(flags)		((flags >> 9) & 0x01)
#define WDNS_FLAGS_RD(flags)		((flags >> 8) & 0x01)
#define WDNS_FLAGS_RA(flags)		((flags >> 7) & 0x01)
#define WDNS_FLAGS_Z(flags)		((flags >> 6) & 0x01)
#define WDNS_FLAGS_AD(flags)		((flags >> 5) & 0x01)
#define WDNS_FLAGS_CD(flags)		((flags >> 4) & 0x01)
#define WDNS_FLAGS_RCODE(flags)		(flags & 0xf)

typedef enum {
	wdns_msg_success,
	wdns_msg_err_invalid_compression_pointer,
	wdns_msg_err_invalid_length_octet,
	wdns_msg_err_invalid_opcode,
	wdns_msg_err_invalid_rcode,
	wdns_msg_err_len,
	wdns_msg_err_malloc,
	wdns_msg_err_name_len,
	wdns_msg_err_name_overflow,
	wdns_msg_err_out_of_bounds,
	wdns_msg_err_overflow,
	wdns_msg_err_parse_error,
	wdns_msg_err_qdcount,
	wdns_msg_err_unknown_opcode,
	wdns_msg_err_unknown_rcode,
} wdns_msg_status;

typedef struct {
	uint16_t		len;
	uint8_t			*data;
} wdns_dns_name_t;

typedef struct {
	uint16_t		len;
	uint8_t			data[];
} wdns_dns_rdata_t;

typedef struct {
	uint16_t		rrtype;
	uint16_t		rrclass;
	wdns_dns_name_t		name;
} wdns_dns_qrr_t;

typedef struct {
	uint16_t		id;
	uint16_t		flags;
	wdns_dns_qrr_t		question;
} wdns_dns_query_t;

typedef struct {
	uint32_t		rrttl;
	uint16_t		rrtype;
	uint16_t		rrclass;
	wdns_dns_name_t		name;
	wdns_dns_rdata_t	*rdata;
} wdns_dns_rr_t;

typedef struct {
	uint32_t		rrttl;
	uint16_t		rrtype;
	uint16_t		rrclass;
	uint16_t		n_rdatas;
	wdns_dns_name_t		name;
	wdns_dns_rdata_t	**rdatas;
} wdns_dns_rrset_t;

typedef struct {
	uint16_t		n_rrsets;
	wdns_dns_rrset_t	**rrsets;
} wdns_dns_rrset_array_t;

typedef struct {
	uint16_t		id;
	uint16_t		flags;
	wdns_dns_qrr_t		question;
	wdns_dns_rrset_array_t	sections[3];
} wdns_dns_message_t;

#define WDNS_MSG_SEC_ANSWER		0
#define WDNS_MSG_SEC_AUTHORITY		1
#define WDNS_MSG_SEC_ADDITIONAL		2
#define WDNS_MSG_SEC_MAX		3

void	wdns_dns_message_clear(wdns_dns_message_t *m);
void	wdns_dns_query_clear(wdns_dns_query_t *q);
void	wdns_dns_rr_clear(wdns_dns_rr_t *rr);
void	wdns_dns_rrset_clear(wdns_dns_rrset_t *rrset);
void	wdns_dns_rrset_array_clear(wdns_dns_rrset_array_t *a);

void	wdns_name_downcase(wdns_dns_name_t *name);
char *	wdns_name_to_str(wdns_dns_name_t *name);
char *	wdns_rdata_to_str(wdns_dns_rdata_t *rdata, uint16_t rrtype, uint16_t rrclass);
size_t	wdns_domain_to_str(const uint8_t *src, char *dst);
size_t	wdns_name_skip(const uint8_t **data, const uint8_t *eod);
void	wdns_print_question_record(FILE *fp, wdns_dns_qrr_t *q);
void	wdns_print_rr(FILE *fp, uint8_t *dname,
		      uint16_t rrtype, uint16_t rrclass, uint32_t rrttl,
		      uint16_t rdlen, const uint8_t *rdata);
void	wdns_print_data(const uint8_t *p, size_t len);
void	wdns_print_message(FILE *fp, wdns_dns_message_t *m);
void	wdns_print_rrset(FILE *fp, wdns_dns_rrset_t *rrset);
void	wdns_print_rrset_array(FILE *fp, wdns_dns_rrset_array_t *a);

wdns_msg_status
wdns_name_len_uncomp(const uint8_t *p, const uint8_t *eop, size_t *sz);

wdns_msg_status
wdns_name_unpack(const uint8_t *p, const uint8_t *eop, const uint8_t *src,
		 uint8_t *dst, size_t *sz);

wdns_msg_status
wdns_parse_message(const uint8_t *op, const uint8_t *eop, wdns_dns_message_t *m);

wdns_msg_status
wdns_parse_message_rr(const uint8_t *p, const uint8_t *eop, const uint8_t *data,
		      size_t *rrsz, wdns_dns_rr_t *rr);

wdns_msg_status
wdns_parse_question_record(const uint8_t *q, const uint8_t *eoq,
			   wdns_dns_qrr_t *question);

wdns_msg_status
wdns_parse_rdata(const uint8_t *p, const uint8_t *eop, const uint8_t *ordata,
		 uint16_t rrtype, uint16_t rrclass, uint16_t rdlen,
		 size_t *alloc_bytes, uint8_t *dst);

wdns_msg_status
wdns_parse_header(const uint8_t *p, size_t len, uint16_t *id, uint16_t *flags,
		  uint16_t *qdcount, uint16_t *ancount, uint16_t *nscount, uint16_t *arcount);