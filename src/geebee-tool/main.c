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

#include "../xbee.h"

void cb_frame (GeebeeXbee *xbee, GeebeePacket *reply, gpointer data);

void read_address(GeebeeXbee *xbee) {
  GeebeeAtCommandRequest *req = geebee_at_command_request_new ("SH", NULL, 0);
  req->packet.frame_id = 'A';

  geebee_xbee_send_async (xbee, (GeebeePacket*)req, NULL, NULL, NULL);
  geebee_packet_unref ((GeebeePacket*)req);
}

void remote_read_address(GeebeeXbee *xbee) {
  GeebeeRemoteAtCommandRequest *req = geebee_remote_at_command_request_new (GEEBEE_BROADCAST_ADDR64, GEEBEE_BROADCAST_ADDR16, "SH", NULL, 0);
  req->packet.frame_id = 'A';

  geebee_xbee_send_async (xbee, (GeebeePacket*)req, NULL, NULL, NULL);
  geebee_packet_unref ((GeebeePacket*)req);
}

int main(int argc, char** argv)
{
  GMainLoop *loop = NULL;
  GError *error = NULL;
  GeebeeXbee *xbee = NULL;
  xbee = geebee_xbee_new ();
  g_signal_connect (xbee, "new-frame", G_CALLBACK (cb_frame), NULL);

  remote_read_address (xbee);
  loop = g_main_loop_new (NULL, 0);
  g_main_loop_run (loop);

  return 0;
}

void
cb_frame (GeebeeXbee *xbee, GeebeePacket *reply, gpointer data)
{
  g_debug ("Got a response of type %d!\n", reply->api_id);
  switch (reply->api_id) {
    case ModemStatus:
      g_printf ("Modem has rebooted due to ");
      switch (((GeebeeModemStatusResponse*)reply)->status) {
        case HardwareReset:
          g_printf ("Hardware reset\n");
          break;
        default:
          g_printf ("Unknown forces!\n");
      };
      break;
    case AtResponse:
      g_printf ("Got a response from AT%s.\n", ((GeebeeAtCommandResponse*)reply)->command);
      break;
    case RemoteAtResponse:
      g_printf ("Remote AT response.\n");
      break;
    default:
      g_printf ("Some unknown packet :(\n");
  }
}

