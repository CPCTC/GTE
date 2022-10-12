#include "buff.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//

static struct buff_node *buff_from_file(FILE *stream, blockset blocks);

//

int open_buff(struct buff_node **buff, blockset *blocks, struct options opt) {
    *buff = NULL;
    *blocks = blockset_init_default();
    if (!*blocks) {
        fprintf(stderr, "Can't init blockset\n");
        return 1;
    }
    if (opt.file) {
        FILE *file = fopen(opt.file, "r");
        if (!file) {
            if (errno == ENOENT)
                return 0;
            fprintf(stderr, "Can't open %s: %s\n", opt.file, strerror(errno));
            return 1;
        }
        *buff = buff_from_file(file, *blocks);
        fclose(file); file = NULL;
        if (!*buff) {
            fprintf(stderr, "Can't create buffer\n");
            return 1;
        }
    }
    return 0;
}

struct buff_node *buff_from_file(FILE *stream, blockset blocks) {
    long pos = ftell(stream);
    if (fseek(stream, 0, SEEK_END)) {
        fprintf(stderr, "Can't seek on stream\n");
        goto err_retn;
    }
    long file_len = ftell(stream);
    fseek(stream, pos, SEEK_SET);
    unsigned long node_len = BUFF_DATA_CAP - 1;
    unsigned long nodes = file_len / node_len + !!(file_len % node_len);
    struct buff_node *buff = malloc(sizeof (struct buff_node) * nodes);
    if (!buff) goto err_retn;
    if (blockset_set(blocks, buff, 0)) goto err_free;
    struct buff_node *prev = NULL;
    for (uint32_t i = 0; i < nodes; i++) {
        size_t len = fread(buff[i].data, 1, node_len, stream);
        buff[i].data[len] = '\0';
        buff[i].prev = prev;
        prev = buff + i;
        buff[i].next = buff + i + 1;
    }
    buff[nodes - 1].next = NULL;
    return buff;

err_free:
    free(buff);
err_retn:
    return NULL;
}
