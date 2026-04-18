use std::cmp::PartialEq;
use rand::{Rng, RngExt};
pub struct GameBoard {
    board: [[u32; 4]; 4]
}
#[derive(PartialEq)]
pub enum Direction {
    Horizontal(i8),
    Vertical(i8),
}
impl GameBoard {
    pub fn new() -> Self {
        GameBoard { board: [[0; 4]; 4] }
    }
    pub fn new_num(&mut self) {
        let mut rng = rand::rng();
        let mut row;
        let mut col;
        loop {
            row = rng.random_range(0..4);
            col = rng.random_range(0..4);
            if self.board[row][col] == 0 { break;}
        }
        let value = if rng.random_bool(0.9) { 2 } else { 4 };
        self.board[row][col] = value;
    }
    pub fn print(&self) {
        for i in 0..4 {
            for j in 0..4 {
                print!("{} ", self.board[i][j]);
            }
            println!();
        }
    }
    pub fn merge(&mut self, direction: Direction) {





    }

    pub fn merge_horizontal(&mut self, x:i32, y:i32, dir:i32) {
        let mut column:i32 = if dir == -1 { (self.board[0].len() - 1) as i32 } else { 0 };

        for row in &mut self.board {
            let mut prev: Option<u32> = None;
            for i in 0..4 {
                let mut curr = &mut row[column as usize];
                if *curr == 0 {
                    *curr = prev;
                    if i != 0 {}
                }
                if curr == prev
                column += dir;
            }

        }
    }
}
