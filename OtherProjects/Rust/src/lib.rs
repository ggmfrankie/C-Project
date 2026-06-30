#[path = "game/game_board.rs"] mod game_board;

use crate::game_board::{Direction, GameBoard};

#[unsafe(no_mangle)]
pub extern "C" fn game_create() -> *mut GameBoard {
    Box::into_raw(Box::new(GameBoard::new()))
}

#[unsafe(no_mangle)]
pub extern "C" fn game_destroy(ptr: *mut GameBoard) {
    if ptr.is_null() { return; }
    unsafe { drop(Box::from_raw(ptr)); }
}

#[unsafe(no_mangle)]
pub extern "C" fn game_input(ptr: *mut GameBoard, dir: i32) {
    if ptr.is_null() { return; }

    let game = unsafe { &mut *ptr };

    let direction = match dir {
        0 => Direction::Up,
        1 => Direction::Left,
        2 => Direction::Down,
        3 => Direction::Right,
        _ => return,
    };

    game.merge(direction);
    game.new_num();
}

#[unsafe(no_mangle)]
pub extern "C" fn game_get_board(ptr: *mut GameBoard, out: *mut i32) {
    if ptr.is_null() || out.is_null() { return; }

    let game = unsafe { &*ptr };
    let out_slice = unsafe { std::slice::from_raw_parts_mut(out, 16) };

    for r in 0..4 {
        for c in 0..4 {
            out_slice[r * 4 + c] = game.board[r][c];
        }
    }
}

fn main() {}