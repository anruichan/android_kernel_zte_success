/*
 *	Generic address resultion entity
 *
 *	Authors:
 *	net_random Alan Cox
 *	net_ratelimit Andi Kleen
 *	in{4,6}_pton YOSHIFUJI Hideaki, Copyright (C)2006 USAGI/WIDE Project
 *
 *	Created by Alexey Kuznetsov <kuznet@ms2.inr.ac.ru>
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/inet.h>
#include <linux/mm.h>
#include <linux/net.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/percpu.h>
#include <linux/init.h>
#include <linux/ratelimit.h>

#include <net/sock.h>
#include <net/net_ratelimit.h>

#include <asm/byteorder.h>
#include <asm/uaccess.h>

int net_msg_warn __read_mostly = 1;
EXPORT_SYMBOL(net_msg_warn);

DEFINE_RATELIMIT_STATE(net_ratelimit_state, 5 * HZ, 10);
#ifdef CONFIG_BOARD_ZTE
/* ZTE_LC_TCP_DEBUG, 20130116 start */
static const char *inet_ntop4(const u_char *src, char *dst, size_t size);
static const char *inet_ntop6(const u_char *src, char *dst, size_t size);
/* ZTE_LC_TCP_DEBUG, 20130116 end */
#endif
/*
 * All net warning printk()s should be guarded by this function.
 */
int net_ratelimit(void)
{
	return __ratelimit(&net_ratelimit_state);
}
EXPORT_SYMBOL(net_ratelimit);

/*
 * Convert an ASCII string to binary IP.
 * This is outside of net/ipv4/ because various code that uses IP addresses
 * is otherwise not dependent on the TCP/IP stack.
 */

__be32 in_aton(const char *str)
{
	unsigned long l;
	unsigned int val;
	int i;

	l = 0;
	for (i = 0; i < 4; i++)	{
		l <<= 8;
		if (*str != '\0') {
			val = 0;
			while (*str != '\0' && *str != '.' && *str != '\n') {
				val *= 10;
				val += *str - '0';
				str++;
			}
			l |= val;
			if (*str != '\0')
				str++;
		}
	}
	return htonl(l);
}
EXPORT_SYMBOL(in_aton);

#define IN6PTON_XDIGIT		0x00010000
#define IN6PTON_DIGIT		0x00020000
#define IN6PTON_COLON_MASK	0x00700000
#define IN6PTON_COLON_1		0x00100000	/* single : requested */
#define IN6PTON_COLON_2		0x00200000	/* second : requested */
#define IN6PTON_COLON_1_2	0x00400000	/* :: requested */
#define IN6PTON_DOT		0x00800000	/* . */
#define IN6PTON_DELIM		0x10000000
#define IN6PTON_NULL		0x20000000	/* first/tail */
#define IN6PTON_UNKNOWN		0x40000000

static inline int xdigit2bin(char c, int delim)
{
	int val;

	if (c == delim || c == '\0')
		return IN6PTON_DELIM;
	if (c == ':')
		return IN6PTON_COLON_MASK;
	if (c == '.')
		return IN6PTON_DOT;

	val = hex_to_bin(c);
	if (val >= 0)
		return val | IN6PTON_XDIGIT | (val < 10 ? IN6PTON_DIGIT : 0);

	if (delim == -1)
		return IN6PTON_DELIM;
	return IN6PTON_UNKNOWN;
}

/**
 * in4_pton - convert an IPv4 address from literal to binary representation
 * @src: the start of the IPv4 address string
 * @srclen: the length of the string, -1 means strlen(src)
 * @dst: the binary (u8[4] array) representation of the IPv4 address
 * @delim: the delimiter of the IPv4 address in @src, -1 means no delimiter
 * @end: A pointer to the end of the parsed string will be placed here
 *
 * Return one on success, return zero when any error occurs
 * and @end will point to the end of the parsed string.
 *
 */
int in4_pton(const char *src, int srclen,
	     u8 *dst,
	     int delim, const char **end)
{
	const char *s;
	u8 *d;
	u8 dbuf[4];
	int ret = 0;
	int i;
	int w = 0;

	if (srclen < 0)
		srclen = strlen(src);
	s = src;
	d = dbuf;
	i = 0;
	while(1) {
		int c;
		c = xdigit2bin(srclen > 0 ? *s : '\0', delim);
		if (!(c & (IN6PTON_DIGIT | IN6PTON_DOT | IN6PTON_DELIM | IN6PTON_COLON_MASK))) {
			goto out;
		}
		if (c & (IN6PTON_DOT | IN6PTON_DELIM | IN6PTON_COLON_MASK)) {
			if (w == 0)
				goto out;
			*d++ = w & 0xff;
			w = 0;
			i++;
			if (c & (IN6PTON_DELIM | IN6PTON_COLON_MASK)) {
				if (i != 4)
					goto out;
				break;
			}
			goto cont;
		}
		w = (w * 10) + c;
		if ((w & 0xffff) > 255) {
			goto out;
		}
cont:
		if (i >= 4)
			goto out;
		s++;
		srclen--;
	}
	ret = 1;
	memcpy(dst, dbuf, sizeof(dbuf));
out:
	if (end)
		*end = s;
	return ret;
}
EXPORT_SYMBOL(in4_pton);

/**
 * in6_pton - convert an IPv6 address from literal to binary representation
 * @src: the start of the IPv6 address string
 * @srclen: the length of the string, -1 means strlen(src)
 * @dst: the binary (u8[16] array) representation of the IPv6 address
 * @delim: the delimiter of the IPv6 address in @src, -1 means no delimiter
 * @end: A pointer to the end of the parsed string will be placed here
 *
 * Return one on success, return zero when any error occurs
 * and @end will point to the end of the parsed string.
 *
 */
 #ifdef CONFIG_BOARD_ZTE
/* ZTE_LC_TCP_DEBUG, 20130116 start */
/* char *
 * inet_ntop(af, src, dst, size)
 * convert a network format address to presentation format.
 * return:
 * pointer to presentation format address (`dst'), or NULL (see errno).
 * author:
 * Paul Vixie, 1996.
 */
const char *
inet_ntop(int af, const void *src, char *dst, size_t size)
{
	switch (af) {
	case AF_INET:
		return inet_ntop4(src, dst, size);
	case AF_INET6:
		return inet_ntop6(src, dst, size);
	default:
		/* errno = EAFNOSUPPORT; */
		return NULL;
	}
	/* NOTREACHED */
}
EXPORT_SYMBOL(inet_ntop);

/* const char *
 * inet_ntop4(src, dst, size)
 * format an IPv4 address, more or less like inet_ntoa()
 * return:
 * `dst' (as a const)
 * notes:
 * (1) uses no statics
 * (2) takes a u_char* not an in_addr as input
 * author:
 * Paul Vixie, 1996.
 */
static const char *
inet_ntop4(const u_char *src, char *dst, size_t size)
{
	static const char fmt[] = "%u.%u.%u.%u";
	char tmp[sizeof "255.255.255.255"];
	int l;

	l = snprintf(tmp, size, fmt, src[0], src[1], src[2], src[3]);
	if (l <= 0 || (size_t)l >= size) {
		/* errno = ENOSPC; */
		return NULL;
	}
	strlcpy(dst, tmp, size);
	return dst;
}
EXPORT_SYMBOL(inet_ntop4);

/* const char *
 * inet_ntop6(src, dst, size)
 * convert IPv6 binary address into presentation (printable) format
 * author:
 * Paul Vixie, 1996.
 */
static const char *
inet_ntop6(const u_char *src, char *dst, size_t size)
{
	/*
	 * Note that int32_t and int16_t need only be "at least" large enough
	 * to contain a value of the specified size.  On some systems, like
	 * Crays, there is no such thing as an integer variable with 16 bits.
	 * Keep this in mind if you think this function should have been coded
	 * to use pointer overlays.  All the world's not a VAX.
	 */
	char tmp[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"];
	char *tp, *ep;
	struct { int base, len; } best, cur;
	u_int words[IN6ADDRSZ / INT16SZ];
	int i;
	int advance;

	/*
	 * Preprocess:
	 * Copy the input (bytewise) array into a wordwise array.
	 * Find the longest run of 0x00's in src[] for :: shorthanding.
	 */
	memset(words, '\0', sizeof(words));
	for (i = 0; i < IN6ADDRSZ; i++)
		words[i / 2] |= (src[i] << ((1 - (i % 2)) << 3));
	best.base = -1;
	best.len  = 0;
	cur.base = -1;
	cur.len  = 0;
	for (i = 0; i < (IN6ADDRSZ / INT16SZ); i++) {
		if (words[i] == 0) {
			if (cur.base == -1)
				cur.base = i, cur.len = 1;
			else
				cur.len++;
		} else {
			if (cur.base != -1) {
				if (best.base == -1 || cur.len > best.len)
					best = cur;
				cur.base = -1;
			}
		}
	}
	if (cur.base != -1) {
		if (best.base == -1 || cur.len > best.len)
			best = cur;
	}
	if (best.base != -1 && best.len < 2)
		best.base = -1;

	/*
	 * Format the result.
	 */
	tp = tmp;
	ep = tmp + sizeof(tmp);
	for (i = 0; i < (IN6ADDRSZ / INT16SZ) && tp < ep; i++) {
		/* Are we inside the best run of 0x00's? */
		if (best.base != -1 && i >= best.base &&
		    i < (best.base + best.len)) {
			if (i == best.base) {
				if (tp + 1 >= ep)
					return NULL;
				*tp++ = ':';
			}
			continue;
		}
		/* Are we following an initial run of 0x00s or any real hex? */
		if (i != 0) {
			if (tp + 1 >= ep)
				return NULL;
			*tp++ = ':';
		}
		/* Is this address an encapsulated IPv4? */
		if (i == 6 && best.base == 0 &&
		    (best.len == 6 || (best.len == 5 && words[5] == 0xffff))) {
			if (!inet_ntop4(src + 12, tp, (size_t)(ep - tp)))
				return NULL;
			tp += strlen(tp);
			break;
		}
		advance = snprintf(tp, ep - tp, "%x", words[i]);
		if (advance <= 0 || advance >= ep - tp)
			return NULL;
		tp += advance;
	}
	/* Was it a trailing run of 0x00's? */
	if (best.base != -1 && (best.base + best.len) == (IN6ADDRSZ / INT16SZ)) {
		if (tp + 1 >= ep)
			return NULL;
		*tp++ = ':';
	}
	if (tp + 1 >= ep)
		return NULL;
	*tp++ = '\0';

	/*
	 * Check for overflow, copy, and we're done.
	 */
	if ((size_t)(tp - tmp) > size) {
		/* errno = ENOSPC; */
		return NULL;
	}
	strlcpy(dst, tmp, size);
	return dst;
}
EXPORT_SYMBOL(inet_ntop6);
/* ZTE_LC_TCP_DEBUG, 20130116 end */
#endif
int in6_pton(const char *src, int srclen,
	     u8 *dst,
	     int delim, const char **end)
{
	const char *s, *tok = NULL;
	u8 *d, *dc = NULL;
	u8 dbuf[16];
	int ret = 0;
	int i;
	int state = IN6PTON_COLON_1_2 | IN6PTON_XDIGIT | IN6PTON_NULL;
	int w = 0;

	memset(dbuf, 0, sizeof(dbuf));

	s = src;
	d = dbuf;
	if (srclen < 0)
		srclen = strlen(src);

	while (1) {
		int c;

		c = xdigit2bin(srclen > 0 ? *s : '\0', delim);
		if (!(c & state))
			goto out;
		if (c & (IN6PTON_DELIM | IN6PTON_COLON_MASK)) {
			/* process one 16-bit word */
			if (!(state & IN6PTON_NULL)) {
				*d++ = (w >> 8) & 0xff;
				*d++ = w & 0xff;
			}
			w = 0;
			if (c & IN6PTON_DELIM) {
				/* We've processed last word */
				break;
			}
			/*
			 * COLON_1 => XDIGIT
			 * COLON_2 => XDIGIT|DELIM
			 * COLON_1_2 => COLON_2
			 */
			switch (state & IN6PTON_COLON_MASK) {
			case IN6PTON_COLON_2:
				dc = d;
				state = IN6PTON_XDIGIT | IN6PTON_DELIM;
				if (dc - dbuf >= sizeof(dbuf))
					state |= IN6PTON_NULL;
				break;
			case IN6PTON_COLON_1|IN6PTON_COLON_1_2:
				state = IN6PTON_XDIGIT | IN6PTON_COLON_2;
				break;
			case IN6PTON_COLON_1:
				state = IN6PTON_XDIGIT;
				break;
			case IN6PTON_COLON_1_2:
				state = IN6PTON_COLON_2;
				break;
			default:
				state = 0;
			}
			tok = s + 1;
			goto cont;
		}

		if (c & IN6PTON_DOT) {
			ret = in4_pton(tok ? tok : s, srclen + (int)(s - tok), d, delim, &s);
			if (ret > 0) {
				d += 4;
				break;
			}
			goto out;
		}

		w = (w << 4) | (0xff & c);
		state = IN6PTON_COLON_1 | IN6PTON_DELIM;
		if (!(w & 0xf000)) {
			state |= IN6PTON_XDIGIT;
		}
		if (!dc && d + 2 < dbuf + sizeof(dbuf)) {
			state |= IN6PTON_COLON_1_2;
			state &= ~IN6PTON_DELIM;
		}
		if (d + 2 >= dbuf + sizeof(dbuf)) {
			state &= ~(IN6PTON_COLON_1|IN6PTON_COLON_1_2);
		}
cont:
		if ((dc && d + 4 < dbuf + sizeof(dbuf)) ||
		    d + 4 == dbuf + sizeof(dbuf)) {
			state |= IN6PTON_DOT;
		}
		if (d >= dbuf + sizeof(dbuf)) {
			state &= ~(IN6PTON_XDIGIT|IN6PTON_COLON_MASK);
		}
		s++;
		srclen--;
	}

	i = 15; d--;

	if (dc) {
		while(d >= dc)
			dst[i--] = *d--;
		while(i >= dc - dbuf)
			dst[i--] = 0;
		while(i >= 0)
			dst[i--] = *d--;
	} else
		memcpy(dst, dbuf, sizeof(dbuf));

	ret = 1;
out:
	if (end)
		*end = s;
	return ret;
}
EXPORT_SYMBOL(in6_pton);

void inet_proto_csum_replace4(__sum16 *sum, struct sk_buff *skb,
			      __be32 from, __be32 to, int pseudohdr)
{
	if (skb->ip_summed != CHECKSUM_PARTIAL) {
		*sum = csum_fold(csum_add(csum_sub(~csum_unfold(*sum), from),
				 to));
		if (skb->ip_summed == CHECKSUM_COMPLETE && pseudohdr)
			skb->csum = ~csum_add(csum_sub(~(skb->csum), from), to);
	} else if (pseudohdr)
		*sum = ~csum_fold(csum_add(csum_sub(csum_unfold(*sum), from),
				  to));
}
EXPORT_SYMBOL(inet_proto_csum_replace4);

void inet_proto_csum_replace16(__sum16 *sum, struct sk_buff *skb,
			       const __be32 *from, const __be32 *to,
			       int pseudohdr)
{
	__be32 diff[] = {
		~from[0], ~from[1], ~from[2], ~from[3],
		to[0], to[1], to[2], to[3],
	};
	if (skb->ip_summed != CHECKSUM_PARTIAL) {
		*sum = csum_fold(csum_partial(diff, sizeof(diff),
				 ~csum_unfold(*sum)));
		if (skb->ip_summed == CHECKSUM_COMPLETE && pseudohdr)
			skb->csum = ~csum_partial(diff, sizeof(diff),
						  ~skb->csum);
	} else if (pseudohdr)
		*sum = ~csum_fold(csum_partial(diff, sizeof(diff),
				  csum_unfold(*sum)));
}
EXPORT_SYMBOL(inet_proto_csum_replace16);

struct __net_random_once_work {
	struct work_struct work;
	struct static_key *key;
};

static void __net_random_once_deferred(struct work_struct *w)
{
	struct __net_random_once_work *work =
		container_of(w, struct __net_random_once_work, work);
	BUG_ON(!static_key_enabled(work->key));
	static_key_slow_dec(work->key);
	kfree(work);
}

static void __net_random_once_disable_jump(struct static_key *key)
{
	struct __net_random_once_work *w;

	w = kmalloc(sizeof(*w), GFP_ATOMIC);
	if (!w)
		return;

	INIT_WORK(&w->work, __net_random_once_deferred);
	w->key = key;
	schedule_work(&w->work);
}

bool __net_get_random_once(void *buf, int nbytes, bool *done,
			   struct static_key *once_key)
{
	static DEFINE_SPINLOCK(lock);
	unsigned long flags;

	spin_lock_irqsave(&lock, flags);
	if (*done) {
		spin_unlock_irqrestore(&lock, flags);
		return false;
	}

	get_random_bytes(buf, nbytes);
	*done = true;
	spin_unlock_irqrestore(&lock, flags);

	__net_random_once_disable_jump(once_key);

	return true;
}
EXPORT_SYMBOL(__net_get_random_once);
