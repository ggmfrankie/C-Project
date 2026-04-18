use crate::game_board::GameBoard;

#[path = "game/game_board.rs"] mod game_board;
fn main() {
    let mut board = GameBoard::new();

    for _ in 0..6 {
        board.new_num();
    }
    board.print();
    
    loop {

    }
}
