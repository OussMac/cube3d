#include "../../cube.h"

void player_rotation(t_parsed_data *pd)
{
    if (pd->keys.pressed[MLX_KEY_RIGHT])
    {
        double old_x = pd->player.bdir.x;
        double old_y = pd->player.bdir.y;
        pd->player.bdir.x = old_x * cos(ROT_SPEED) - old_y * sin(ROT_SPEED); // matrix rotation calculation [x', y'] = [x, y] * [x * cos(R) + y * sin(R), x * -sin(R) + y * cos(R)] 
        pd->player.bdir.y = old_x * sin(ROT_SPEED) + old_y * cos(ROT_SPEED);
    }
    if (pd->keys.pressed[MLX_KEY_LEFT])
    {
        double old_x = pd->player.bdir.x;
        double old_y = pd->player.bdir.y;
        pd->player.bdir.x = old_x * cos(-ROT_SPEED) - old_y * sin(-ROT_SPEED);
        pd->player.bdir.y = old_x * sin(-ROT_SPEED) + old_y * cos(-ROT_SPEED);
    }
    int delta_x = pd->mouse.x - (WIDTH / 2);
    if (delta_x != 0)
    {
        double angle = delta_x * MOUSE_SENSITIVITY;
        double old_x = pd->player.bdir.x;
        double old_y = pd->player.bdir.y;
        pd->player.bdir.x = old_x * cos(angle) - old_y * sin(angle);
        pd->player.bdir.y = old_x * sin(angle) + old_y * cos(angle);
    }
    mlx_set_mouse_pos(pd->mlx, WIDTH / 2, HEIGHT / 2);
}

static bool is_collision(char **map_grid, double x, double y)
{
    // check for wall collision with a small margin
    if (map_grid[(int)y][(int)x] == '1' || map_grid[(int)y][(int)x] == 'D')
        return (true);
    // horizontal collision
    if (map_grid[(int)y][(int)(x + COLLISION)] == '1'
        || map_grid[(int)y][(int)(x - COLLISION)] == '1')
        return (true);
    if (map_grid[(int)y][(int)(x + COLLISION)] == 'D'
        || map_grid[(int)y][(int)(x - COLLISION)] == 'D')
        return (true);
    // vertical collision
    if (map_grid[(int)(y + COLLISION)][(int)x] == '1' 
        || map_grid[(int)(y - COLLISION)][(int)x] == '1')
        return (true);
    if (map_grid[(int)(y + COLLISION)][(int)x] == 'D' 
        || map_grid[(int)(y - COLLISION)][(int)x] == 'D')
        return (true);
    return (false);
}


static void forward_backward(t_parsed_data *pd)
{
    double sprint_mult = (pd->keys.pressed[MLX_KEY_LEFT_SHIFT] ||
                          pd->keys.pressed[MLX_KEY_RIGHT_SHIFT]) ? 2.0 : 1.0;
    double move_speed;

    if (pd->keys.pressed[MLX_KEY_W])
    {
        move_speed = (double)SPEED;  /* assign first */
        move_speed *= sprint_mult;   /* then multiply */
        pd->player.new_pos.x += pd->player.bdir.x * move_speed;
        pd->player.new_pos.y += pd->player.bdir.y * move_speed;
        if (!is_collision(pd->map_grid, pd->player.new_pos.x, pd->player.bpos.y))
            pd->player.bpos.x = pd->player.new_pos.x;
        if (!is_collision(pd->map_grid, pd->player.bpos.x, pd->player.new_pos.y))
            pd->player.bpos.y = pd->player.new_pos.y;
    }
    if (pd->keys.pressed[MLX_KEY_S])
    {
        move_speed = (double)SPEED;
        move_speed *= sprint_mult;
        pd->player.new_pos.x -= pd->player.bdir.x * move_speed;
        pd->player.new_pos.y -= pd->player.bdir.y * move_speed;
        if (!is_collision(pd->map_grid, pd->player.new_pos.x, pd->player.bpos.y))
            pd->player.bpos.x = pd->player.new_pos.x;
        if (!is_collision(pd->map_grid, pd->player.bpos.x, pd->player.new_pos.y))
            pd->player.bpos.y = pd->player.new_pos.y;
    }
}

static void strafe(t_parsed_data *pd)
{
    double sprint_mult = (pd->keys.pressed[MLX_KEY_LEFT_SHIFT] ||
                          pd->keys.pressed[MLX_KEY_RIGHT_SHIFT]) ? 2.0 : 1.0;
    double move_speed;

    if (pd->keys.pressed[MLX_KEY_A])
    {
        move_speed = (double)STRAFE_SPEED;
        move_speed *= sprint_mult;
        pd->player.new_pos.x += pd->player.bdir.y * move_speed;
        pd->player.new_pos.y += -pd->player.bdir.x * move_speed;
        if (!is_collision(pd->map_grid, pd->player.new_pos.x, pd->player.bpos.y))
            pd->player.bpos.x = pd->player.new_pos.x;
        if (!is_collision(pd->map_grid, pd->player.bpos.x, pd->player.new_pos.y))
            pd->player.bpos.y = pd->player.new_pos.y;
    }
    if (pd->keys.pressed[MLX_KEY_D])
    {
        move_speed = (double)STRAFE_SPEED;
        move_speed *= sprint_mult;
        pd->player.new_pos.x += -pd->player.bdir.y * move_speed;
        pd->player.new_pos.y += pd->player.bdir.x * move_speed;
        if (!is_collision(pd->map_grid, pd->player.new_pos.x, pd->player.bpos.y))
            pd->player.bpos.x = pd->player.new_pos.x;
        if (!is_collision(pd->map_grid, pd->player.bpos.x, pd->player.new_pos.y))
            pd->player.bpos.y = pd->player.new_pos.y;
    }
}


void player_movement(t_parsed_data *pd)
{
    pd->player.new_pos.x = pd->player.bpos.x;
    pd->player.new_pos.y = pd->player.bpos.y;
    forward_backward(pd);
    strafe(pd);
}
