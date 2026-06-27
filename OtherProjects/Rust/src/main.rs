use game_board::Direction;

use crate::game_board::GameBoard;
use device_query::{DeviceQuery, DeviceState, Keycode};

#[path = "game/game_board.rs"] mod game_board;
#[path = "lib.rs"] mod lib;
fn main() {
    let mut board = GameBoard::new();
    let device_state = DeviceState::new();

    board.new_num();
    board.print();
    loop {
        let keys: Vec<Keycode> = device_state.get_keys();

        let dir = match keys.first() {
            Some(Keycode::W) => Direction::Up,
            Some(Keycode::A) => Direction::Left,
            Some(Keycode::S) => Direction::Down,
            Some(Keycode::D) => Direction::Right,
            _ => continue,
        };

        board.merge(dir);
        board.new_num();
        board.print();
        std::thread::sleep(std::time::Duration::from_millis(120));

    }
}
