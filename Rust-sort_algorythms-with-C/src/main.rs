extern crate rand;
use std::time::Instant;

pub type Sorttype = i32;

#[link(name="optimal_sort", kind="static")]
extern {
    fn optimal_sort(datavec: *const Sorttype, log2len: u32);
}

pub fn optimal_sort_rs(m: &Vec<Sorttype>, log2len: u32) -> Vec<Sorttype> {
    let mut m_clone = m.clone();
    unsafe { optimal_sort(m_clone.as_mut_ptr(), log2len); }
    return m_clone;
}


fn main() {
    let log2len = 20;
    let mut m: Vec<Sorttype> = Vec::new();
    for _ in 0..1<<log2len {
	m.push(rand::random());
    }

    let start_time = Instant::now();
    let _ord = optimal_sort_rs(&m, log2len);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("\nOptimal sort elapsed time: {} ms", milliseconds);
}
