/* edits */
static uint8_t edit_name_data[128], edit_status_data[128], edit_addid_data[TOX_FRIEND_ADDRESS_SIZE * 2], edit_addmsg_data[1024], edit_msg_data[1024];

static void edit_name_onenter(void)
{
    uint8_t *data = edit_name_data;
    uint16_t length = edit_name.length;

    if(!length) {
        return;
    }

    memcpy(self.name, data, length);
    self.name_length = length;

    tox_postmessage(TOX_SETNAME, length, 0, self.name);//!

    redraw();
}

static void edit_status_onenter(void)
{
    uint8_t *data = edit_status_data;
    uint16_t length = edit_status.length;

    if(!length) {
        return;
    }

    void *p = realloc(self.statusmsg, length);
    if(!p) {
        return;
    }

    self.statusmsg = p;
    memcpy(self.statusmsg, data, length);
    self.statusmsg_length = length;

    tox_postmessage(TOX_SETSTATUSMSG, length, 0, self.statusmsg);//!

    redraw();
}

static void edit_msg_onenter(void)
{
    uint16_t length = edit_msg.length;
    if(length == 0) {
        return;
    }

    if(sitem->item == ITEM_FRIEND) {
        FRIEND *f = sitem->data;

        MESSAGE *msg = malloc(length + 6);
        msg->flags = 1;
        msg->length = length;
        memcpy(msg->msg, edit_msg_data, length);

        /*uint16_t *data = malloc(length + 8);
        data[0] = 0;
        data[1] = length;
        memcpy((void*)data + 4, edit_msg_data, length);*/

        friend_addmessage(f, msg);

        //
        void *d = malloc(length);
        memcpy(d, edit_msg_data, length);

        tox_postmessage(TOX_SENDMESSAGE, (f - friend), length, d);
    } else {
        GROUPCHAT *g = sitem->data;

        void *d = malloc(length);
        memcpy(d, edit_msg_data, length);

        tox_postmessage(TOX_SENDMESSAGEGROUP, (g - group), length, d);
    }

    edit_clear();

    redraw();
}

EDIT edit_name = {
    .panel = {
        .type = PANEL_EDIT,
        .x = 5 * SCALE,
        .y = SCALE * 14,
        .height = SCALE * 12,
        .width = -SCROLL_WIDTH - 5 * SCALE
    },
    .maxlength = 128,
    .data = edit_name_data,
    .onenter = edit_name_onenter,
},

edit_status = {
    .panel = {
        .type = PANEL_EDIT,
        .x = 5 * SCALE,
        .y = SCALE * 38,
        .height = SCALE * 12,
        .width = -SCROLL_WIDTH - 5 * SCALE
    },
    .maxlength = 128,
    .data = edit_status_data,
    .onenter = edit_status_onenter,
},

edit_addid = {
    .panel = {
        .type = PANEL_EDIT,
        .x = 5 * SCALE,
        .y = SCALE * 14,
        .height = SCALE * 12,
        .width = -SCROLL_WIDTH - 5 * SCALE
    },
    .maxlength = sizeof(edit_addid_data),
    .data = edit_addid_data,
},

edit_addmsg = {
    .panel = {
        .type = PANEL_EDIT,
        .x = 5 * SCALE,
        .y = SCALE * 38,
        .height = SCALE * 42,
        .width = -SCROLL_WIDTH - 5 * SCALE
    },
    .multiline = 0,//1,
    .maxlength = sizeof(edit_addmsg_data),
    .data = edit_addmsg_data,
},

edit_msg = {
    .panel = {
        .type = PANEL_EDIT,
        .x = 0,
        .y = -94,
        .height = 84,
        .width = 0
    },
    .multiline = 0,//1,
    .maxlength = sizeof(edit_msg_data),
    .data = edit_msg_data,
    .onenter = edit_msg_onenter,
};
