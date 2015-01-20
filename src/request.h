/**
 * Copyright (C) 2015 Torrie Fischer <tdfischer@phrobo.net>
 *
 * This file is part of GeeBee, the GLib XBee/ZigBee library.
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

#ifndef __GEEBEE_REQUEST_H__
#define __GEEBEE_REQUEST_H__

#include "address64.h"

#include <glib.h>

G_BEGIN_DECLS

typedef enum _GeebeeApiId {
  AtRequest = 0x08,
  AtResponse = 0x88,
  ModemStatus = 0x8a,
  RemoteAtRequest = 0x17,
  RemoteAtResponse = 0x97
} GeebeeApiId;

typedef struct _GeebeePacket {
  GeebeeApiId api_id;
  guint8 frame_id;
  guint _ref;
} GeebeePacket;

typedef struct _GeebeeAtCommandRequest {
  GeebeePacket packet;
  char command[2];
  guint8 *value;
  guint8 value_length;
} GeebeeAtCommandRequest;

typedef enum _GeebeeAtCommandResponseStatus {
  Ok = 0,
  Error = 1,
  InvalidCommand = 2,
  InvalidParam = 3
} GeebeeAtCommandResponseStatus;

typedef struct _GeebeeAtCommandResponse {
  GeebeePacket packet;
  char command[2];
  GeebeeAtCommandResponseStatus status;
  guint8 *value;
  guint8 value_length;
} GeebeeAtCommandResponse;

typedef enum _GeebeeModemStatus {
  HardwareReset = 0,
  WatchdogReset = 1,
  Associated = 2,
  Disassociated = 3,
  SynchronizationLost = 4,
  CoordinatorRealignment = 5,
  CoordinatorStarted = 6
} GeebeeModemStatus;

typedef struct _GeebeeModemStatusResponse {
  GeebeePacket packet;
  GeebeeModemStatus status;
} GeebeeModemStatusResponse;

typedef struct _GeebeeRemoteAtCommandRequest {
  GeebeePacket packet;
  GeebeeAddress64 address64;
  GeebeeAddress16 address16;
  unsigned char options;
  char command[2];
  char data[72];
  unsigned char dataLength;
} GeebeeRemoteAtCommandRequest;

typedef enum _GeebeeRemoteAtStatus {
  RemoteAtOk = 0,
  RemoteAtError = 1,
  RemoteAtInvalidCommand = 2,
  RemoteAtInvalidParameter = 3,
  RemoteAtTxFailed = 4
} GeebeeRemoteAtStatus;

typedef struct _GeebeeRemoteAtResponse {
  GeebeePacket packet;
  GeebeeAddress64 address64;
  GeebeeAddress16 address16;
  char command[2];
  GeebeeRemoteAtStatus status;
  char data[4];
} GeebeeRemoteAtResponse;

gsize geebee_packet_build (GeebeePacket *pkt, gchar *buf);
void geebee_packet_unref (GeebeePacket *pkt);
GeebeePacket *geebee_packet_ref (GeebeePacket *pkt); 
GeebeePacket *geebee_packet_parse (gchar *buf, gsize bufsize);
guchar geebee_packet_checksum (gchar *buf, gsize length);

GeebeeAtCommandRequest *geebee_at_command_request_new (const gchar *cmd, const gchar *param, gsize param_length);
GeebeeAtCommandResponse *geebee_at_command_response_new (const gchar *cmd, guchar status, gchar *param, gsize param_length);
GeebeeModemStatusResponse *geebee_modem_status_response_new (GeebeeModemStatus status);
GeebeeRemoteAtCommandRequest *geebee_remote_at_command_request_new (GeebeeAddress64 dst64, GeebeeAddress16 dst16, const gchar *cmd, const gchar *param, gsize param_length);
GeebeeRemoteAtResponse *geebee_remote_at_response_new (GeebeeAddress64 src64, GeebeeAddress16 src16, const gchar *cmd, guchar status, const gchar *param);

G_END_DECLS

#endif // __GEEBEE_REQUEST_H__
