/*
 * Copyright © 2015 Information Technology Authority (ITA) <foss@ita.gov.om>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include <locale.h>
#include <glib.h>

#include "raqm.h"

static gchar *direction = NULL;
static gchar *text = NULL;
static gchar *features = NULL;
static gchar *font = NULL;
static gchar *fonts = NULL;
static GOptionEntry entries[] =
{
  { "text", 0, 0, G_OPTION_ARG_STRING, &text, "The text to be displayed", "TEXT" },
  { "font", 0, 0, G_OPTION_ARG_STRING, &font, "The font file", "FONT" },
  { "fonts", 0, 0, G_OPTION_ARG_STRING, &fonts, "Font files and ranges for multi-fonts: <fontfile1> start:length, ...", "FONTS" },
  { "direction", 0, 0, G_OPTION_ARG_STRING, &direction, "The text direction", "DIR" },
  { "font-features", 0, 0, G_OPTION_ARG_STRING, &features, "The font features ", "FEATURES" },
  { NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
};

int
main (int argc, char *argv[])
{
  FT_Library library;
  FT_Face face;

  raqm_t *rq;
  raqm_glyph_t *glyphs;
  size_t count;
  raqm_direction_t dir;

  GError *error = NULL;
  GOptionContext *context;

  setlocale (LC_ALL, "");
  context = g_option_context_new ("- Test libraqm");
  g_option_context_add_main_entries (context, entries, NULL);

  if (!g_option_context_parse (context, &argc, &argv, &error))
  {
    g_print ("Option parsing failed: %s\n", error->message);
    return 1;
  }

  g_option_context_free (context);

  if (text == NULL || (font == NULL && fonts == NULL))
  {
    g_print ("Text or font is missing.\n\n%s",
             g_option_context_get_help (context, TRUE, NULL));
    return 1;
  }

  dir = RAQM_DIRECTION_DEFAULT;
  if (direction && strcmp(direction, "rtl") == 0)
    dir = RAQM_DIRECTION_RTL;
  else if (direction && strcmp(direction, "ltr") == 0)
    dir = RAQM_DIRECTION_LTR;

  rq = raqm_create ();
  assert (raqm_set_text_utf8 (rq, text, strlen (text)));
  assert (raqm_set_par_direction (rq, dir));
  assert (!FT_Init_FreeType (&library));

  if (fonts)
  {
      gchar **list = g_strsplit (fonts, ",", -1);
      for (gchar **p = list; p != NULL && *p != NULL; p++) {

          gchar **sublist = g_strsplit (*p, " ", -1);
          gchar **range;
          int s, l;
          assert (!FT_New_Face (library, sublist[0], 0, &face));
          assert (!FT_Set_Char_Size (face, face->units_per_EM, 0, 0, 0));
          range = g_strsplit (sublist[1], ":", -1);
          s = atoi(range[0]);
          l = atoi(range[1]);
          assert (raqm_set_freetype_face_range(rq, face, s, l));

          g_strfreev (range);
          g_strfreev (sublist);
      }
      g_strfreev (list);
  } else {
      assert (!FT_New_Face (library, font, 0, &face));
      assert (!FT_Set_Char_Size (face, face->units_per_EM, 0, 0, 0));
      assert (raqm_set_freetype_face (rq, face));
  }

  if (features)
  {
    gchar **list = g_strsplit (features, ",", -1);
    for (gchar **p = list; p != NULL && *p != NULL; p++)
      assert (raqm_add_font_feature (rq, *p, -1));
    g_strfreev (list);
  }

  assert (raqm_layout (rq));

  glyphs = raqm_get_glyphs (rq, &count);
  assert (glyphs != NULL);

  raqm_destroy (rq);
  FT_Done_Face (face);
  FT_Done_FreeType (library);

  return 0;
}
