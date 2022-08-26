#![allow(dead_code)]

use std::{
    alloc::{alloc, dealloc, realloc, Layout},
    fmt::{Debug, Display},
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
        let layout = Layout::array::<MaybeUninit<T>>(size).unwrap();
        let memoria = unsafe { alloc(layout) };
        Pila {
            layout,
            memoria,
            maximum: size,
            current: 0,
            ty: PhantomData,
        }
    }

    pub fn resize(&mut self, size: usize) {
        let new = Layout::array::<MaybeUninit<T>>(size).unwrap();
        self.memoria = unsafe { realloc(self.memoria, self.layout, new.size()) };
        self.maximum = size;
        self.layout = new;
    }

    pub fn fix(&mut self) {
        self.resize(self.current)
    }

    pub fn top(&self) -> Option<&T> {
        if self.current == 0 {
            None
        } else {
            unsafe { Some(self.get_slice()[self.current - 1].assume_init_ref()) }
        }
    }

    pub fn pop(&mut self) -> Option<T> {
        if self.current == 0 {
            None
        } else {
            self.current -= 1;
            let i = self.current;
            unsafe {
                let value = self.get_slice_mut()[i].assume_init_read();
                self.get_slice_mut()[i] = std::mem::MaybeUninit::uninit();
                Some(value)
            }
        }
    }

    pub fn push(&mut self, value: T) {
        (self.current == self.maximum).then(|| self.resize(self.maximum + 1));
        let c = self.current;
        unsafe { self.get_slice_mut()[c].write(value) };
        self.current += 1;
    }
}

impl<T> Drop for Pila<T> {
    fn drop(&mut self) {
        unsafe { dealloc(self.memoria, self.layout) }
    }
}

impl<T: Display> Display for Pila<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let slice = unsafe { self.get_slice() };
        (0..self.current)
            .into_iter()
            .try_for_each(|i| write!(f, " {:4} |", unsafe { slice[i].assume_init_ref() }))
    }
}

impl<T: Debug> Debug for Pila<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let slice = unsafe { self.get_slice() };
        (0..self.current)
            .into_iter()
            .try_for_each(|i| write!(f, " {:4?} |", unsafe { slice[i].assume_init_ref() }))?;
        (self.current..self.maximum)
            .into_iter()
            .try_for_each(|_| write!(f, " {:4} |", "null"))
    }
}

fn main() {
    let mut pila: Pila<String> = Pila::new(0);
    let mut i = 1;
    while i < 10 {
        pila.push("Hola".to_owned());
        println!("{pila:?}");
        i += 1;
    }

    while pila.pop().is_some() {
        // pila.fix();
        println!("{pila:?}");
    }
}
