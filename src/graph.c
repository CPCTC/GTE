#include "graph.h"
#include "graph/priv.h"
#include "graph/debug.h"
#include "graph/dev.h"
#include "graph/inst.h"
#include "graph/pipe/tri.h"
#include "graph/rpass.h"
#include "graph/win.h"
#include <stdlib.h>

GRAPH graph_init(void) {
    Graph *g = malloc(sizeof (Graph));
    if (!g) goto err_retn;

    g->win = win_open();
    if (!g->win) goto err_free_g;

    g->inst = inst_create();
    if (!g->inst) goto err_close_win;

#ifdef DEBUG
    g->msgr = debug_start(g->inst);
    if (!g->msgr) goto err_free_inst;
#endif

    g->srf = srf_init(g->inst, g->win);
    if (!g->srf) goto err_stop_debug;

    g->pdev = select_pdev(g->inst, g->srf, &g->q_infos);
    if (!g->pdev) goto err_free_srf;

    g->dev = dev_init(g->pdev, g->q_infos, g->qs);
    if (!g->dev) goto err_free_srf;

    if (sch_init(g->dev, g->pdev, g->srf, &g->sch, &g->srf_info))
        goto err_free_dev;

    if (img_init(g->dev, g->sch, g->srf_info, &g->imgs))
        goto err_free_sch;

    if (shaders_init(g->dev, &g->shaders))
        goto err_free_img;

    if (rpass_init(g->dev, g->srf_info.fmt.format, &g->rpass))
        goto err_free_shaders;

    if (triangle_pipe_init(g->dev, &g->shaders,
                g->rpass, 0,
                g->srf_info.extent,
                &g->tri_layout, &g->tri_pipe))
        goto err_free_rpass;

    if (frames_init(g->dev, &g->imgs, g->rpass, g->srf_info.extent, &g->frames))
        goto err_free_pipe;

    if (pools_init(g->dev, &g->q_infos, &g->pools))
        goto err_free_frames;

    if (triangle_cmds_init(g->dev, &g->pools, g->rpass,
                &g->frames, g->tri_pipe, g->srf_info.extent,
                &g->tri_cmds))
        goto err_free_pools;

    if (sync_init(g->dev, g->imgs.nimgs, &g->sync))
        goto err_free_tri_cmds;

    shaders_destroy(g->dev, &g->shaders);
    return g;

err_free_tri_cmds:
    triangle_cmds_destroy(g->dev, &g->pools, &g->tri_cmds);
err_free_pools:
    pools_destroy(g->dev, &g->pools);
err_free_frames:
    frames_destroy(g->dev, &g->frames);
err_free_pipe:
    triangle_pipe_destroy(g->dev, &g->tri_layout, &g->tri_pipe);
err_free_rpass:
    rpass_destroy(g->dev, &g->rpass);
err_free_shaders:
    shaders_destroy(g->dev, &g->shaders);
err_free_img:
    img_destroy(g->dev, &g->imgs);
err_free_sch:
    sch_destroy(g->dev, &g->sch);
err_free_dev:
    dev_destroy(&g->dev, g->qs);
err_free_srf:
    srf_destroy(g->inst, &g->srf);
err_stop_debug:
#ifdef DEBUG
    debug_stop(g->inst, &g->msgr);
err_free_inst:
#endif
    inst_destroy(&g->inst);
err_close_win:
    win_close(&g->win);
err_free_g:
    free(g);
err_retn:
    return NULL;
}

void graph_destroy(GRAPH *hg) {
    Graph *g = *hg;
    vkDeviceWaitIdle(g->dev);
    sync_destroy(g->dev, &g->sync);
    triangle_cmds_destroy(g->dev, &g->pools, &g->tri_cmds);
    pools_destroy(g->dev, &g->pools);
    frames_destroy(g->dev, &g->frames);
    triangle_pipe_destroy(g->dev, &g->tri_layout, &g->tri_pipe);
    rpass_destroy(g->dev, &g->rpass);
    img_destroy(g->dev, &g->imgs);
    sch_destroy(g->dev, &g->sch);
    dev_destroy(&g->dev, g->qs);
    srf_destroy(g->inst, &g->srf);
#ifdef DEBUG
    debug_stop(g->inst, &g->msgr);
#endif
    inst_destroy(&g->inst);
    win_close(&g->win);
    free(g);
    *hg = NULL;
}
