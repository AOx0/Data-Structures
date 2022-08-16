#![allow(dead_code)]

use std::{
    alloc::{alloc, dealloc, Layout},
    fmt::Display,
    marker::PhantomData, cell::RefCell, mem::MaybeUninit,
};

pub struct Pila<T: Sized> {
    memoria: *mut u8,
    current: usize,
    maximum: usize,
    layout: Layout,
    ty: PhantomData<T>,
}

impl<T: Sized + Clone + Default> Pila<T> {
    unsafe fn get_slice(&self) -> &mut [MaybeUninit<RefCell<T>>] {
        core::slice::from_raw_parts_mut(self.memoria as *mut MaybeUninit<RefCell<T>>, self.maximum as usize)
    }

    pub fn new(size: usize) -> Self {
        unsafe {
            let pila = Pila {
                layout: Layout::array::<MaybeUninit<RefCell<T>>>(size).unwrap(),
                memoria: alloc(Layout::array::<MaybeUninit<RefCell<T>>>(size).unwrap()),
                maximum: size,
                current: 0,
                ty: PhantomData,
            };
            
            let iter = pila.get_slice();

            for i in 0..size {
                iter[i] = std::mem::MaybeUninit::uninit();
                iter[i].write(RefCell::new(T::default()));
            };

            pila

        }
    }

    pub fn top(&self) -> Option<&mut T> {
        unsafe {
            if self.current == 0 {
                None
            } else {
                Some(self.get_slice()[self.current - 1].assume_init_mut().get_mut())
            }
        }
    }

    pub fn pop(&mut self) -> Option<T> {
        unsafe {
            if self.current == 0 {
                None
            } else {
                let value = self.get_slice()[self.current - 1].assume_init_mut().take();
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
                self.get_slice()[self.current].assume_init_mut().replace(value);
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

impl<T: Display + Clone + Default> Display for Pila<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        unsafe {
            let slice = self.get_slice(); 
            for i in 0..self.maximum {
                match i {
                    x if x < self.current => write!(f, " {:4} |", slice[x].assume_init_ref().borrow())?,
                    _ => write!(f, " {:4} |", "null")?,
                }
            }
        }

        Ok(())
    }
}

fn main() {
    let mut pila: Pila<String> = Pila::new(10);
    let mut i = 1;
    while let Ok(_) = pila.push("Hola".to_owned()) {
        print!("Agregado : {:4} -- ", i * 10);
        println!("{pila}");
        i += 1;
    }

    *pila.top().unwrap() = "Test".to_owned();

    while let Some(value) = pila.pop() {
        print!("Eliminado: {value:4} -- ");
        println!("{pila}");
    }
}
