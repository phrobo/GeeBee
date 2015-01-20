/**
 * Copyright (C) 2015 Torrie Fischer <tdfischer@phrobo.net>
 *
 * This file is part of GeeBee, the GLib ZigBee/XBee library.
 *
 * GeeBee is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GeeBee is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GeeBee. If not, see <http://www.gnu.org/licenses/>
 */

#include "request.h"
#include "address64.h"
#include <memory.h>

GeebeeAtCommandRequest *
geebee_at_command_request_new (const gchar *cmd, const gchar *param, gsize param_length)
{
  GeebeeAtCommandRequest *req = g_new0 (GeebeeAtCommandRequest, 1);
  req->packet.api_id = AtRequest;
  req->command[0] = cmd[0];
  req->command[1] = cmd[1];

  if (param_length) {
    req->value = g_new0 (guint8, param_length);
    memcpy (req->value, param, param_length);
    req->value_length = param_length;
  }

  geebee_packet_ref ((GeebeePacket*)req);

  return req;
}

void
geebee_packet_unref (GeebeePacket *pkt)
{
  pkt->_ref--;
  if (pkt->_ref == 0) {
    g_free (pkt);
  }
}

GeebeePacket *
geebee_packet_ref (GeebeePacket *pkt)
{
  pkt->_ref++;
}

static gsize
build_at_packet (GeebeeAtCommandRequest *pkt, gchar *buf)
{
  int i;
  buf[0] = AtRequest;
  buf[1] = pkt->packet.frame_id;
  buf[2] = pkt->command[0];
  buf[3] = pkt->command[1];
  for (i = 0; i < pkt->value_length;i++) {
    buf[i + 4] = pkt->value[i];
  }
  return 4 + pkt->value_length;
}

static gsize
read_addr64 (GeebeeAddress64 *addr, gchar *buf)
{
  gsize i = 0;
  guint32 bits = 0;
  for (i; i < sizeof (bits); i++) {
    bits |= buf[i] & 0xff;
    bits = bits << 8;
  }
  bits |= buf[i] & 0xff;
  addr->msb = bits;
  bits = 0;
  for (i; i < sizeof (bits) * 2; i++) {
    bits |= buf[i] & 0xff;
    bits = bits << 8;
  }
  bits |= buf[i] & 0xff;
  addr->lsb = bits;
  return i;
}

static gsize
write_addr64 (GeebeeAddress64 *addr, gchar *buf)
{
  gssize i = 0;
  guint32 bits = __bswap_32 (addr->msb);
  for (i; i < sizeof (bits); i++) {
    buf[i] = bits & 0xff;
    bits = bits >> 8;
  }
  bits = __bswap_32 (addr->lsb);
  for (i; i < sizeof (bits) * 2; i++) {
    buf[i] = bits & 0xff;
    bits = bits >> 8;
  }

  return i + 1;
}

static gsize
read_addr16 (GeebeeAddress16 *addr, gchar *buf)
{
  gsize i = 0;
  guint16 bits = 0;
  for (i; i < sizeof (bits); i++) {
    bits |= buf[i] & 0xff;
    bits = bits << 8;
  }
  bits |= buf[i] & 0xff;
  addr->addr = bits;
  return i;
}

static gsize
write_addr16 (GeebeeAddress16 *addr, gchar *buf)
{
  gssize i = 0;
  guint16 bits = __bswap_16 (addr->addr);
  for (i; i < sizeof (bits); i++) {
    buf[i] = bits & 0xff;
    bits = bits >> 8;
  }
  return i + 1;
}

static gsize
build_remote_at_packet (GeebeeRemoteAtCommandRequest *pkt, gchar *buf)
{
  int i;
  int s = 0;
  buf[s++] = RemoteAtRequest;
  buf[s++] = pkt->packet.frame_id;
  s += write_addr64 (&pkt->address64, &buf[s]) - 1;
  s += write_addr16 (&pkt->address16, &buf[s]) - 1;
  buf[s++] = 0;
  buf[s++] = pkt->command[0];
  buf[s++] = pkt->command[1];
  return s;
}

gsize
geebee_packet_build (GeebeePacket *pkt, gchar *buf)
{
  gsize length;
  switch (pkt->api_id) {
    case AtRequest:
      length = build_at_packet ((GeebeeAtCommandRequest*)pkt, &buf[3]);
      break;
    case RemoteAtRequest:
      length = build_remote_at_packet ((GeebeeRemoteAtCommandRequest*)pkt, &buf[3]);
      break;
    default:
      g_assert (FALSE);
  }
  buf[0] = 0x7e;
  buf[1] = length >> 8 & 0xff;
  buf[2] = length & 0xff;
  buf[length+3] = geebee_packet_checksum (&buf[3], length);
  return length + 4;
}

GeebeeAtCommandResponse *
geebee_at_command_response_new (const gchar *command, guchar status, gchar *value, gsize value_length)
{
  GeebeeAtCommandResponse *resp = g_new0 (GeebeeAtCommandResponse, 1);
  resp->packet.api_id = AtResponse;
  resp->command[0] = command[0];
  resp->command[1] = command[1];
  resp->status = status;
  resp->value = value;
  resp->value_length = value_length;
  geebee_packet_ref ((GeebeePacket*)resp);
  return resp;
}

GeebeeModemStatusResponse *
geebee_modem_status_response_new (GeebeeModemStatus status)
{
  GeebeeModemStatusResponse *resp = g_new0 (GeebeeModemStatusResponse, 1);
  resp->packet.api_id = ModemStatus;
  resp->status = status;
}

GeebeePacket*
parse_at_packet (guchar *buf, gsize bufsize)
{
  g_assert_cmphex (buf[0], ==, AtResponse);

  GeebeeAtCommandResponse *resp;
  gchar *param = 0;
  gsize param_length = 0;

  if (param_length > 0) {
    param = &buf[5];
  }

  resp = geebee_at_command_response_new (&buf[2], buf[4], param, param_length);
  resp->packet.frame_id = buf[1];
  return (GeebeePacket*)resp;
}

GeebeePacket*
parse_status_packet (guchar *buf, gsize bufsize)
{
  g_assert_cmphex (buf[0], ==, ModemStatus);

  return (GeebeePacket*)geebee_modem_status_response_new (buf[1]);
}

static int
min(int a, int b)
{
  return (a < b) ? a : b;
}

GeebeePacket*
parse_remote_at_packet (guchar *buf, gsize bufsize)
{
  g_assert_cmphex (buf[0], ==, RemoteAtResponse);
  GeebeeRemoteAtResponse *resp;
  guint idx = 1;
  GeebeeAddress64 addr64;
  GeebeeAddress16 addr16;
  char command[2];
  char param[4];
  memset (&param[0], 0, sizeof (param));
  GeebeeRemoteAtStatus status;

  idx += read_addr64 (&addr64, &buf[idx]);
  idx += read_addr16 (&addr16, &buf[idx]);
  memcpy (&command[0], &buf[idx], sizeof (command));
  idx += sizeof (command);
  status = buf[idx];
  idx++;
  if (idx < bufsize) {
    memcpy (&param[0], &buf[idx], min (sizeof (param), bufsize - idx));
  }
  resp = geebee_remote_at_response_new (addr64, addr16, command, status, param);
  resp->packet.frame_id = buf[1];
  return (GeebeePacket*)resp;
}

guchar
geebee_packet_checksum (gchar *buf, gsize length)
{
  guint checksum = 0;
  gsize i;
  for (i = 0; i < length; i++) {
    checksum += (guchar)buf[i];
  }

  return 0xff - (checksum & 0xff);
}

GeebeePacket *
geebee_packet_parse (gchar *buf, gsize bufsize)
{
  gchar start = buf[0];
  gsize length = (buf[1] << 8) + (buf[2]);
  guchar calculated_checksum = geebee_packet_checksum (&buf[3], length);

  g_assert_cmphex (start, ==, 0x7e);
  g_assert_cmphex (calculated_checksum, ==, (guchar)buf[length+3]);

  switch ((guchar)buf[3]) {
    case AtResponse:
      return parse_at_packet (&buf[3], length);
    case ModemStatus:
      return parse_status_packet (&buf[3], length);
    case RemoteAtResponse:
      return parse_remote_at_packet (&buf[3], length);
    default:
      g_assert (FALSE);
  }
}

GeebeeRemoteAtResponse *
geebee_remote_at_response_new (GeebeeAddress64 src64, GeebeeAddress16 src16, const gchar *cmd, guchar status, const gchar *param)
{
  GeebeeRemoteAtResponse *resp = g_new0 (GeebeeRemoteAtResponse, 1);
  resp->packet.api_id = RemoteAtResponse;
  memcpy (&resp->address64, &src64, sizeof (src64));
  memcpy (&resp->address16, &src16, sizeof (src16));
  resp->command[0] = cmd[0];
  resp->command[1] = cmd[1];
  resp->status = status;
  memcpy (&resp->data[0], param, sizeof (resp->data));
  return resp;
}

GeebeeRemoteAtCommandRequest *
geebee_remote_at_command_request_new (GeebeeAddress64 dst64,
                                      GeebeeAddress16 dst16,
                                      const gchar *cmd,
                                      const gchar *param,
                                      gsize param_length)
{
  GeebeeRemoteAtCommandRequest *req = g_new0 (GeebeeRemoteAtCommandRequest, 1);
  req->packet.api_id = RemoteAtRequest;
  memcpy (&req->address64, &dst64, sizeof (dst64));
  memcpy (&req->address16, &dst16, sizeof (dst16));
  memcpy (req->command, cmd, sizeof (req->command));
  memcpy (req->data, param, param_length);
  return req;
}
