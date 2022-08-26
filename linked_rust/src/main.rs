#![allow(dead_code)]

use std::{alloc::{Layout, alloc, dealloc}, mem::MaybeUninit, fmt::Debug, marker::PhantomData, cmp::Ordering};

#[derive(Debug)]
struct Node<T: Debug> {
    value: T,
    curr: *mut u8,
    next: Option<*mut u8>,
    prev: Option<*mut u8>
}

impl<T: Debug> Node<T> {
    pub fn child(&mut self, value: T) {
        unsafe {
            let l = Layout::new::<MaybeUninit<Node<T>>>();
            let raw_new_node = alloc(l);
            let new_node = raw_new_node as *mut MaybeUninit<Node<T>>;

            new_node.write(MaybeUninit::new(Node {
                curr: raw_new_node,
                prev: Some(self.curr),
                next: None,
                value
            }));

            // Save new node address
            self.next = Some(raw_new_node); 
        }
    }

    pub fn new(value: T) -> Self {
       unsafe {
            let l = Layout::new::<MaybeUninit<Node<T>>>();
            let raw_new_node = alloc(l);
            let new_node = raw_new_node as *mut MaybeUninit<Node<T>>;

            new_node.write(MaybeUninit::new(Node {
                curr: raw_new_node,
                prev: None,
                next: None,
                value
            }));

            // Save new node addres
            (*new_node).assume_init_read() 
       } 
    }
}

impl<A: Debug> FromIterator<A> for LinkedList<A> {
    fn from_iter<T: IntoIterator<Item = A>>(iter: T) -> Self {
       let mut linked: LinkedList<A> = LinkedList::new();
       for element in iter {
           linked.push(element);
       }
       linked
    }
}

impl<T: Debug> Drop for Node<T> {
    fn drop(&mut self) {
        if let Some(next) = self.next {
            let next = next as *mut MaybeUninit<Node<T>>;
            unsafe { drop( (*next).assume_init_read() ) }
        } 
        unsafe { dealloc(self.curr, Layout::new::<T>()) }
    }
}

#[derive(Debug)]
struct LinkedList<T> 
    where T: Debug
{
    current_last: Option<*mut u8>,
    fisrt_node: Option<*mut u8>,
    first: MaybeUninit<Node<T>>,
    lenght: usize,
    marker: PhantomData<T>
}

impl<T: Debug> LinkedList<T> {
    fn new() -> Self {
        LinkedList {
            first: MaybeUninit::uninit(),
            current_last: None,
            fisrt_node: None,
            lenght: 0,
            marker: PhantomData
        } 
    }

    fn push(&mut self, value: T) {
        match self.lenght {
            0 => {
                let new = Node::new(value);
                let curr = new.curr;
                self.first.write(new);
                self.current_last = Some(curr);
                self.fisrt_node = Some(curr);
            },
            _ => {
                let last = self.current_last.unwrap() as *mut MaybeUninit<Node<T>>;
                unsafe { (*last).assume_init_mut().child(value) };
                unsafe { self.current_last = (*last).assume_init_ref().next};
            }
        }
        self.lenght += 1;
    }
}

impl<T: Debug> Drop for LinkedList<T> {
   fn drop(&mut self) {
       if self.fisrt_node.is_some() {
            unsafe { (*(self.fisrt_node.unwrap() as *mut MaybeUninit<Node<T>>)).assume_init_read(); }
       }
   } 
}



impl<T: Debug> LinkedList<T> {
    fn iter(&self) -> LinkedListIter<T> {
        LinkedListIter { item: self, last: None, offset: 0 }
    }

    fn iter_mut(&mut self) -> LinkedListIterMut<T> {
        LinkedListIterMut { item: self, last: None, offset: 0 }
    }
 
}

struct LinkedListIter<'a, T: Debug> {
    item: &'a LinkedList<T>,
    last: Option<*mut u8>,
    offset: usize
} 

impl<'a, T: Debug> Iterator for LinkedListIter<'a, T> {
    type Item = &'a Node<T>;
    fn next(&mut self) -> Option<Self::Item> {
        if self.item.lenght == 0 {
            None
        } else {
            self.last = match self.offset.partial_cmp(&0).unwrap() { 
                Ordering::Equal => {
                    self.offset += 1;
                    Some(self.item.fisrt_node.unwrap())
                }, 
                Ordering::Greater => {
                    let last = self.last.unwrap() as *mut MaybeUninit<Node<T>>;
                    if let Some(next) = unsafe { (*last).assume_init_ref().next } {
                        self.offset += 1;
                        Some(next)
                    } else {
                       None 
                    }
                },
                Ordering::Less => None
            };

            if self.last.is_some() {
                let last = self.last.unwrap() as *mut MaybeUninit<Node<T>>;
                Some(unsafe { (*last).assume_init_ref() })
            } else {
                None
            }
        }
        

    }
}
struct LinkedListIterMut<'a, T: Debug> {
    item: &'a mut LinkedList<T>,
    last: Option<*mut u8>,
    offset: usize
} 

impl<'a, T: Debug> Iterator for LinkedListIterMut<'a, T> {
    type Item = &'a mut Node<T>;
    fn next(&mut self) -> Option<Self::Item> {
        if self.item.lenght == 0 {
            None
        } else {
            self.last = match self.offset.partial_cmp(&0).unwrap() { 
                Ordering::Equal => {
                    self.offset += 1;
                    Some(self.item.fisrt_node.unwrap())
                }, 
                Ordering::Greater => {
                    let last = self.last.unwrap() as *mut MaybeUninit<Node<T>>;
                    if let Some(next) = unsafe { (*last).assume_init_ref().next } {
                        self.offset += 1;
                        Some(next)
                    } else {
                       None 
                    }
                },
                Ordering::Less => None
            };

            if self.last.is_some() {
                let last = self.last.unwrap() as *mut MaybeUninit<Node<T>>;
                Some(unsafe { (*last).assume_init_mut() })
            } else {
                None
            }
        }
        

    }
}

fn main() {
    let mut n = LinkedList::from_iter(["Hola".to_owned(), "Adios".to_owned()]);

    for (i, element) in n.iter().enumerate() {
        println!("{i}: {}", element.value);
    } 

    for element in n.iter_mut() {
        element.value = "Jaja".to_owned();
    }

   for (i, element) in n.iter().enumerate() {
        println!("{i}: {}", element.value);
    }
}
