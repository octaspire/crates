/*
  Crates ‐ extensible 3D puzzle game.
  Copyright (C) 2008‐2010  Octaspire (www.octaspire.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "omessagestack.h"
#include "oerror.h"
#include "ostring.h"
#include "otextmanager.h"

const oint32 omessagestack_initialbasey = 0;

OMessageStack *omessagestack_new(OTextManager *textmanager)
{
  OMessageStack *messagestack = (OMessageStack*)oerror_malloc(sizeof(OMessageStack));
  messagestack->textmanager = textmanager;
  messagestack->messages = optrvector_new(0, (OPtrVectorElementCallback)ostring_release);
  messagestack->showtime = 10;
  messagestack->timeleft = messagestack->showtime;
  messagestack->basey = omessagestack_initialbasey;
  return messagestack;
}

void omessagestack_release(OMessageStack *messagestack)
{
  if (!messagestack)
    return;

  optrvector_release(messagestack->messages);
  memset(messagestack, 0, sizeof(OMessageStack));
  oerror_free(messagestack);
}

void omessagestack_push(OMessageStack *messagestack, const ochar *message)
{
  OString *string = ostring_newstr(message);
  optrvector_pushback(messagestack->messages, string);
  messagestack->timeleft = messagestack->showtime;
  messagestack->basey = omessagestack_initialbasey;
}

void omessagestack_pop(OMessageStack *messagestack)
{
  OString *string = (OString*)optrvector_popback(messagestack->messages);
  ostring_release(string);
  messagestack->timeleft = messagestack->showtime;
  messagestack->basey = omessagestack_initialbasey;
}

void omessagestack_update(OMessageStack *messagestack, const onumber tpf)
{
  if (messagestack->timeleft <= 0)
  {
    if (messagestack->basey > -16)
    {
      messagestack->basey -= 1;
      return;
    }

    omessagestack_pop(messagestack);
    return;
  }

  messagestack->timeleft -= tpf;
}

static void omessagestack_rendermessage(void *element, ouint32 index, void *userdata)
{
  oint32 len = optrvector_length(((OMessageStack*)userdata)->messages);
  const ochar *message = ostring_tocstr((const OString*)element);
  otextmanager_printortho(((OMessageStack*)userdata)->textmanager, 0, ((OMessageStack*)userdata)->basey + ((len - index - 1)*16), message, 0);
}

void omessagestack_render(OMessageStack *messagestack)
{
  optrvector_rforeachi(messagestack->messages, omessagestack_rendermessage, messagestack);
}

void omessagestack_setshowtime(OMessageStack *messagestack, const oint32 showtime)
{
  messagestack->showtime = showtime;
  messagestack->timeleft = messagestack->showtime;
}
