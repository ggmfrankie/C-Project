use rand::RngExt;
pub struct GameBoard {
    pub(crate) board: [[i32; 4]; 4]
}
#[derive(PartialEq)]
pub enum Direction {
    Up = 0,
    Left = 1,
    Down = 2,
    Right = 3,
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
        println!();
        for i in 0..4 {
            for j in 0..4 {
                let num = self.board[i][j];
                print!("|{:4}", if num == 0 {"".to_string()} else {num.to_string()});
            }
            println!("|");
        }
        println!();
    }
    pub fn merge(&mut self, direction: Direction) {
        match direction {
            Direction::Left => {
                self.merge_left();
                self.combine_left();
                self.merge_left();
            },
            Direction::Right => {
                self.rotate_180();
                self.merge_left();
                self.combine_left();
                self.merge_left();
                self.rotate_180();
            },
            Direction::Up => {
                self.rotate_neg90();
                self.merge_left();
                self.combine_left();
                self.merge_left();
                self.rotate_pos90();
            },
            Direction::Down => {
                self.rotate_pos90();
                self.merge_left();
                self.combine_left();
                self.merge_left();
                self.rotate_neg90();
            }
        }

    }
    fn merge_left(&mut self) {
        for row in self.board.iter_mut() {
            let mut new_values: [i32; 4] = [0; 4];
            let mut index = 0;
            for &i in row.iter() {
                if i != 0 {
                    new_values[index] = i;
                    index += 1;
                }
            }
            *row = new_values;
        }
    }

    fn combine_left(&mut self){
        for row in self.board.iter_mut() {
            let mut prev = row[0];

            for i in 1..4 {
                let curr = row[i];
                if curr == prev {
                    row[i] = 0;
                    row[i-1] *= 2;
                }
                prev = curr;
            }
        }
    }
    fn rotate_pos90(&mut self){
        let mut new = [[0;4];4];

        for r in 0..4 {
            for c in 0..4 {
                new[c][3-r] = self.board[r][c];
            }
        }
        self.board = new;
    }

    fn rotate_neg90(&mut self){
        let mut new = [[0;4];4];

        for r in 0..4 {
            for c in 0..4 {
                new[3-c][r] = self.board[r][c];
            }
        }
        self.board = new;
    }

    fn rotate_180(&mut self){
        let mut new = [[0;4];4];

        for r in 0..4 {
            for c in 0..4 {
                new[3-r][3-c] = self.board[r][c];
            }
        }
        self.board = new;
    }
}


