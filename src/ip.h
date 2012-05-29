/*
 * ip.h - work with IPv4 headers and ICMP packets
 *
 * Written by
 *  Christian Vogelgsang <chris@vogelgsang.org>
 *
 * This file is part of plip2slip.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#ifndef IP_H
#define IP_H

#include "global.h"

/* list of IP protocol numbers */
#define IP_PROTOCOL_ICMP    0x01
#define IP_PROTOCOL_TCP     0x06
#define IP_PROTOCOL_UDP     0x11

#define IP_MIN_HDR_SIZE     20

#define IP_CHECKSUM_OFF     10
   
#define UDP_CHECKSUM_OFF    6
#define TCP_CHECKSUM_OFF    16

/* generic checksum calculation for header checksum and others */
extern u16 ip_calc_checksum(const u08 *buf, u08 offset, int num_words);
extern void ip_adjust_checksum(u08 *buf, u08 offset, const u08 *old_ip, const u08 *net_ip); 

/* header checksum */
extern u16 ip_hdr_get_checksum(const u08 *buf);
extern u16 ip_hdr_calc_checksum(const u08 *buf);
extern u08 ip_hdr_validate_checksum(const u08 *buf);
extern void ip_hdr_set_checksum(u08 *buf);

/* query values */
extern const u08 *ip_get_src_ip(const u08 *buf);
extern const u08 *ip_get_tgt_ip(const u08 *buf);
extern u16 ip_get_total_length(const u08 *buf);
extern u08 ip_get_hdr_length(const u08 *buf);
extern u08 ip_is_ipv4_protocol(const u08 *buf, u08 protocol);
extern u08 ip_get_protocol(const u08 *buf);

extern void ip_set_src_ip(u08 *buf, const u08 *ip);
extern void ip_set_tgt_ip(u08 *buf, const u08 *ip);

/* create packet */
extern u08 ip_begin_pkt(u08 *buf, const u08 *tgt_ip, u08 protocol);
extern void ip_finish_pkt(u08 *buf, u16 size);

#endif