/*
 * cmd_table.c - command table
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

#include "cmd_table.h"
#include "uartutil.h"

#include "net/net.h"
#include "param.h"

COMMAND(cmd_quit)
{
  return CMD_QUIT;
}

COMMAND(cmd_version)
{
  uart_send_pstring(PSTR("v" VERSION "\r\n"));
  return CMD_OK;
}

COMMAND(cmd_param_dump)
{
  param_dump();
  return CMD_OK;
}

COMMAND(cmd_param_save)
{
  u08 result = param_save();
  if(result == PARAM_OK) {
    return CMD_OK;
  } else {
    return CMD_MASK_ERROR | result;
  }
}

COMMAND(cmd_param_load)
{
  u08 result = param_load();
  if(result == PARAM_OK) {
    return CMD_OK;
  } else {
    return CMD_MASK_ERROR | result;
  }
}

COMMAND(cmd_param_reset)
{
  param_reset();
  return CMD_OK;
}

COMMAND(cmd_param_set_ip)
{
  if(argc != 2) {
    return CMD_WRONG_ARGC;
  }

  u08 ip_type = argv[0][1];
  u08 *ip = 0;
  switch(ip_type) {
    case 'n': ip = param.ip_net_mask; break;
    case 'g': ip = param.ip_gw_addr; break;
    case 'e': ip = param.ip_eth_addr; break;
    case 'p': ip = param.ip_plip_addr; break;
    case 'a': ip = param.ip_amiga_addr; break;
    default: return CMD_PARSE_ERROR;
  }
  
  if(net_parse_ip(argv[1], ip)) {
    return CMD_OK;
  } else {
    return CMD_PARSE_ERROR;
  }
}

COMMAND(cmd_param_set_mac)
{
  if(argc != 2) {
    return CMD_WRONG_ARGC;
  }
  if(net_parse_mac(argv[1], param.mac_addr)) {
    return CMD_OK;
  } else {
    return CMD_PARSE_ERROR;
  }
}

cmd_table_t cmd_table[] = {
  { CMD_NAME("q"), cmd_quit },
  { CMD_NAME("v"), cmd_version },
  { CMD_NAME("p"), cmd_param_dump },
  { CMD_NAME("ps"), cmd_param_save },
  { CMD_NAME("pl"), cmd_param_load },
  { CMD_NAME("pr"), cmd_param_reset },
  { CMD_NAME("nm"), cmd_param_set_mac },
  { CMD_NAME("nn"), cmd_param_set_ip },
  { CMD_NAME("ng"), cmd_param_set_ip },
  { CMD_NAME("ne"), cmd_param_set_ip },
  { CMD_NAME("np"), cmd_param_set_ip },
  { CMD_NAME("na"), cmd_param_set_ip },
  { 0,0 } // last entry
};