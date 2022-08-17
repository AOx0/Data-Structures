#![allow(dead_code)]

use std::{
    alloc::{alloc, dealloc, Layout},
    fmt::Display,
    marker::PhantomData,
};

struct Pila<T> {
    memoria: Vec<T>,
    current: usize,
}


impl<T: Sized + Clone> Pila<T> {
    pub fn new(size: usize) -> Self {
        Pila {
            memoria: Vec::with_capacity(size),
            current: 0,
        }
    }

    pub fn top(&self) -> Option<T> {
        if self.current == 0 {
            None
        } else {
            Some(self.memoria[self.current - 1].clone())
        }
    }

    pub fn pop(&mut self) -> Option<T> {
        if self.current == 0 {
            None
        } else {
            let value = self.memoria[self.current - 1].clone();
            self.current -= 1;
            Some(value)
        }
    }

    pub fn push(&mut self, value: T) {
        self.memoria.push(value);
        self.current += 1;
    }
}

impl<T: Display> Display for Pila<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for i in 0..self.current {
            match i {
                x if x < self.current => write!(f, " {:4} |", self.memoria[x])?,
                _ => write!(f, " {:4} |", "null")?,
            }
        }

        Ok(())
    }
}

fn main() {
    let mut pila: Pila<String> = Pila::new(0);
    let mut i = 1;
    while i < 10 {
        pila.push("Hola".to_owned()); 
        println!("{pila}");
        i += 1;
    }

    while let Some(_) = pila.pop() {
        println!("{pila}");
    }
}
