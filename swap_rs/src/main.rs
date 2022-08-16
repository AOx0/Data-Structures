use std::io::{stdout, Write};

fn main() {
    let (mut x, mut y) = (5, 10);
    let mut a = stdout();
    write!(a, "{x}{y}").unwrap(); 
    (y, x) = (x, y);
    write!(a, "{x}{y}").unwrap();
}
