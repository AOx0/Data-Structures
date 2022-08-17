#![allow(dead_code)]

use std::{
    alloc::{alloc, dealloc, Layout},
    fmt::Display,
    marker::PhantomData,
};

struct Pila<T: Sized> {
    memoria: *mut u8,
    current: usize,
    maximum: usize,
    layout: Layout,
    ty: PhantomData<T>,
}

impl<T: Sized + Copy> Pila<T> {
    unsafe fn get_slice(&self) -> &mut [T] {
        core::slice::from_raw_parts_mut(self.memoria as *mut T, self.maximum as usize)
    }

    pub fn new(size: usize) -> Self {
        unsafe {
            Pila {
                layout: Layout::array::<T>(size).unwrap(),
                memoria: alloc(Layout::array::<T>(size).unwrap()),
                maximum: size,
                current: 0,
                ty: PhantomData,
            }
        }
    }

    pub fn top(&self) -> Option<T> {
        unsafe {
            if self.current == 0 {
                None
            } else {
                Some(self.get_slice()[self.current - 1])
            }
        }
    }

    pub fn pop(&mut self) -> Option<T> {
        unsafe {
            if self.current == 0 {
                None
            } else {
                let value = self.get_slice()[self.current - 1];
                self.current -= 1;
                Some(value)
            }
        }
    }

    pub fn push(&mut self, value: T) -> Result<(), ()> {
        unsafe {
            if self.current == self.maximum {
                Err(())
            } else {
                self.get_slice()[self.current] = value;
                self.current += 1;
                Ok(())
            }
        }
    }
}

impl<T> Drop for Pila<T> {
    fn drop(&mut self) {
        unsafe { dealloc(self.memoria, self.layout) }
    }
}

impl<T: Display> Display for Pila<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        unsafe {
            let slice: &mut [T] =
                core::slice::from_raw_parts_mut(self.memoria as *mut T, self.maximum as usize);
            for i in 0..self.maximum {
                match i {
                    x if x < self.current => write!(f, " {:4} |", slice[x])?,
                    _ => write!(f, " {:4} |", "null")?,
                }
            }
        }

        Ok(())
    }
}

fn main() {
    let mut pila: Pila<usize> = Pila::new(10);
    let mut i = 1;
    while let Ok(_) = pila.push(i * 10) {
        print!("Agregado : {:4} -- ", i * 10);
        println!("{pila}");
        i += 1;
    }

    while let Some(value) = pila.pop() {
        print!("Eliminado: {value:4} -- ");
        println!("{pila}");
    }
}
