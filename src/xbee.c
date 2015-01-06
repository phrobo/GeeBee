#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xbee.h"

#include <gio/gunixinputstream.h>
#include <gio/gunixoutputstream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

typedef struct _GeebeeXbeePrivate GeebeeXbeePrivate;

struct _GeebeeXbeePrivate
{
  GAsyncQueue *pkt_buffer;
  guint fd;
  GSource *input_source;
  GInputStream *input_stream;
  GOutputStream *output_stream;
};

#define GEEBEE_XBEE_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GEEBEE_XBEE_TYPE, GeebeeXbeePrivate))

static void geebee_xbee_class_init (GeebeeXbeeClass *klass);
static void geebee_xbee_init       (GeebeeXbee *self);
static void geebee_xbee_dispose    (GObject *object);
static void geebee_xbee_finalize   (GObject *object);
static void geebee_xbee_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void geebee_xbee_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

G_DEFINE_TYPE (GeebeeXbee, geebee_xbee, G_TYPE_OBJECT);

enum {
  PROP_ZERO,
  N_PROPERTIES
};

enum {
  SIGNAL_0,
  SIGNAL_NEW_FRAME,
  N_SIGNALS
};

static int obj_signals[N_SIGNALS] = { 0, };

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void
geebee_xbee_class_init (GeebeeXbeeClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (GeebeeXbeePrivate));

  object_class->dispose = geebee_xbee_dispose;
  object_class->finalize = geebee_xbee_finalize;
  object_class->set_property =  geebee_xbee_set_property;
  object_class->get_property =  geebee_xbee_get_property;
  /*g_object_class_install_properties (object_class,
      N_PROPERTIES,
      obj_properties);*/

  obj_signals[SIGNAL_NEW_FRAME] = 
    g_signal_new ("new-frame",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  G_TYPE_POINTER);
}

static void
geebee_xbee_set_property (GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec)
{
  GeebeeXbee *self = GEEBEE_XBEE (object);
  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
geebee_xbee_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
  GeebeeXbee *self = GEEBEE_XBEE (object);
  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static gboolean
cb_read (GInputStream *input, gpointer user_data)
{
  GeebeeXbee *self = GEEBEE_XBEE (user_data);
  gchar buf[1024];
  int length;
  g_debug ("Got a read event!");

  // Read header
  g_input_stream_read_all (self->priv->input_stream, buf, 3, NULL, NULL, NULL);
  length = (buf[1] << 8) + buf[2];
  g_input_stream_read_all (self->priv->input_stream, &buf[3], length + 1, NULL, NULL, NULL);
  GeebeePacket *resp = geebee_packet_parse (buf, sizeof (buf));
  g_signal_emit (self, obj_signals[SIGNAL_NEW_FRAME], 0, resp, NULL);
  geebee_packet_unref (resp);

  return G_SOURCE_CONTINUE;
}

static void
geebee_xbee_init (GeebeeXbee *self)
{
  GeebeeXbeePrivate *priv;
  struct termios options;
  priv = self->priv = GEEBEE_XBEE_GET_PRIVATE (self);

  priv->fd = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  tcgetattr (priv->fd, &options);
  cfsetispeed (&options, B9600);
  options.c_cflag |= (CLOCAL | CREAD);
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_cflag &= ~CRTSCTS;

  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  options.c_iflag &= ~(IXON | IXOFF | IXANY);

  if (tcsetattr (priv->fd, TCSANOW, &options) < 0)
    g_error ("Error: %d", errno);

  priv->input_stream = g_unix_input_stream_new (priv->fd, FALSE);
  priv->output_stream = g_unix_output_stream_new (priv->fd, FALSE);
  priv->input_source = g_pollable_input_stream_create_source (G_POLLABLE_INPUT_STREAM (priv->input_stream), NULL);
  g_source_set_callback (priv->input_source, (GSourceFunc)cb_read, self, NULL);
  g_source_attach (priv->input_source, NULL);
}

static void
geebee_xbee_dispose (GObject *object)
{
  G_OBJECT_CLASS (geebee_xbee_parent_class)->dispose (object);
}

static void
geebee_xbee_finalize (GObject *object)
{
  G_OBJECT_CLASS (geebee_xbee_parent_class)->finalize (object);
}

void
geebee_xbee_send_async (GeebeeXbee *self,
                        GeebeePacket *req,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
  guchar buf[1024];
  gsize length;

  length = geebee_packet_build (req, buf);

  g_output_stream_write (self->priv->output_stream, buf, length, NULL, NULL);
  g_debug ("Wrote frame");
}

GeebeeXbee*
geebee_xbee_new ()
{
  return g_object_new (GEEBEE_XBEE_TYPE, NULL);
}
