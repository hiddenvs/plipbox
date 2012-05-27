/*
 * net.c - tool functions for network protocols
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

#include "net.h"
#include "util.h"
#include "uartutil.h"

static u08 mac_addr[6];
static u08 ip_addr[4];
static u08 gw_addr[4];
static u08 net_mask[4];

void net_init(const u08 mac[6], const u08 ip[4], const u08 gw[4], const u08 nm[4])
{
  net_copy_mac(mac, mac_addr);
  net_copy_ip(ip, ip_addr);
  net_copy_ip(gw, gw_addr);
  net_copy_ip(nm, net_mask);
}

const u08* net_get_mac(void)
{
  return mac_addr;
}
  
const u08* net_get_ip(void)
{
  return ip_addr;
}

const u08* net_get_gateway(void)
{
  return gw_addr;
}

const u08* net_get_netmask(void)
{
  return net_mask;
}

void net_copy_my_mac(u08 *out)
{
  net_copy_mac(mac_addr, out);
}

void net_copy_my_ip(u08 *out)
{
  net_copy_ip(ip_addr, out);
}

u08 net_compare_my_mac(const u08 *in)
{
  return net_compare_mac(mac_addr, in);
}

u08 net_compare_my_ip(const u08 *in)
{
  return net_compare_ip(ip_addr, in);
}

void net_copy_mac(const u08 *in, u08 *out)
{
  for(int i=0;i<6;i++) {
    out[i] = in[i];
  }
}

void net_copy_any_mac(u08 *out)
{
  for(int i=0;i<6;i++) {
    out[i] = 0xff;
  }
}

void net_copy_zero_mac(u08 *out)
{
  for(int i=0;i<6;i++) {
    out[i] = 0;
  }
}

void net_copy_ip(const u08 *in, u08 *out)
{
  for(int i=0;i<4;i++) {
    out[i] = in[i];
  }
}

void net_copy_zero_ip(u08 *out)
{
  for(int i=0;i<4;i++) {
    out[i] = 0;
  }
}

u16  net_get_word(const u08 *buf)
{
  return (u16)buf[0] << 8 | (u16)buf[1];
}

void net_put_word(u08 *buf, u16 value)
{
  buf[0] = (u08)(value >> 8);
  buf[1] = (u08)(value & 0xff);
}

u32  net_get_long(const u08 *buf)
{
  return (u32)buf[0] << 24 | (u32)buf[1] << 16 | (u32)buf[2] << 8 | (u32)buf[3];
}

void net_put_long(u08 *buf, u32 value)
{
  buf[0] = (u08)(value >> 24);
  buf[1] = (u08)((value >> 16) & 0xff);
  buf[2] = (u08)((value >> 8) & 0xff);
  buf[3] = (u08)(value & 0xff);
}

static char *mac_str = "00:00:00:00:00:00";
static char *ip_str = "000.000.000.000";

void net_dump_mac(const u08 *in)
{
  int pos = 0;
  for(int i=0;i<6;i++) {
    byte_to_hex(in[i],(u08 *)(mac_str+pos));
    pos += 3;
  }
  uart_send_string(mac_str);
}

static void byte_to_dec(u08 value, u08 *out)
{
  u08 h = value / 100;
  u08 t = value % 100;
  u08 o = t % 10;
  t = t / 10;
  out[0] = '0' + h;
  out[1] = '0' + t;
  out[2] = '0' + o;
}

void net_dump_ip(const u08 *in)
{
  int pos = 0;
  for(int i=0;i<4;i++) {
    byte_to_dec(in[i],(u08 *)(ip_str+pos));
    pos += 4;
  }
  uart_send_string(ip_str);
}

u08  net_compare_mac(const u08 *a, const u08 *b)
{
  for(int i=0;i<6;i++) {
    if(a[i] != b[i]) {
      return 0;
    }
  }
  return 1;
}

u08 net_compare_any_mac(const u08 *in)
{
  for(int i=0;i<6;i++) {
    if(in[i] != 0xff) {
      return 0;
    }
  }
  return 1;
}

u08  net_compare_ip(const u08 *a, const u08 *b)
{
  for(int i=0;i<4;i++) {
    if(a[i] != b[i]) {
      return 0;
    }
  }
  return 1;
}

u08 net_is_my_subnet(const u08 *ip)
{
  for(int i=0;i<4;i++) {
    u08 diff = ip[i] ^ ip_addr[i];
    diff &= net_mask[i];
    if(diff != 0) {
      return 0;
    }
  }
  return 1;
}

