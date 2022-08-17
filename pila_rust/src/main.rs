#![allow(dead_code)]

use std::{
    alloc::{alloc, dealloc, realloc, Layout},
    fmt::Display,
    marker::PhantomData,
    mem::MaybeUninit,
};

pub struct Pila<T: Sized> {
    memoria: *mut u8,
    current: usize,
    maximum: usize,
    layout: Layout,
    ty: PhantomData<T>,
}

impl<T: Sized> Pila<T> {
    unsafe fn get_slice(&self) -> &[MaybeUninit<T>] {
        core::slice::from_raw_parts(self.memoria as *mut MaybeUninit<T>, self.maximum as usize)
    }

    unsafe fn get_slice_mut(&mut self) -> &mut [MaybeUninit<T>] {
        core::slice::from_raw_parts_mut(self.memoria as *mut MaybeUninit<T>, self.maximum as usize)
    }

    pub fn new(size: usize) -> Self {
        unsafe {
            let layout = Layout::array::<MaybeUninit<T>>(size).unwrap();
            let memoria = alloc(layout);
            Pila {
                layout,
                memoria,
                maximum: size,
                current: 0,
                ty: PhantomData,
            }
        }
    }

    pub unsafe fn resize(&mut self, size: usize) {
        let new = Layout::array::<MaybeUninit<T>>(size).unwrap();
        self.memoria = realloc(self.memoria, self.layout, new.size());
        self.maximum = self.current;
        self.layout = new;
    }

    pub fn fix(&mut self) {
        unsafe { self.resize(self.current) }
    }

    pub fn top(&self) -> Option<&T> {
        unsafe {
            if self.current == 0 {
                None
            } else {
                Some(self.get_slice()[self.current - 1].assume_init_ref())
            }
        }
    }

    pub fn pop(&mut self) -> Option<T> {
        unsafe {
            if self.current == 0 {
                None
            } else {
                self.current -= 1;
                let i = self.current;
                let value = self.get_slice_mut()[i].assume_init_read();
                self.get_slice_mut()[i] = std::mem::MaybeUninit::uninit();
                Some(value)
            }
        }
    }

    pub fn push(&mut self, value: T) {
        unsafe {
            if self.current == self.maximum {
                self.maximum += 1;
                let new = Layout::array::<MaybeUninit<T>>(self.maximum).unwrap();
                self.memoria = realloc(self.memoria, self.layout, new.size());
                self.layout = new;
            }
            let c = self.current;
            self.get_slice_mut()[c].write(value);
            self.current += 1;
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
            let slice = self.get_slice();
            for i in 0..self.current {
                write!(f, " {:4} |", slice[i].assume_init_ref())?
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

    while pila.pop().is_some() {
        //        pila.fix();
        println!("{pila}");
    }
}
