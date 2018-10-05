extern crate rand;
mod optimal_sort;
mod optimal_sort_recursive;

use std::time::Instant;

fn main() {
    let mut m: Vec<optimal_sort::Sorttype> = Vec::new();
    for _ in 0..1<<20 {
	m.push(rand::random());
    }

    let mut m_clone = m.clone();
    let start_time = Instant::now();
    let _ord = optimal_sort_recursive::merge_sort_recursive(&mut m_clone, 0, m.len()-1);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("\nRecursive sort elapsed time: {} ms", milliseconds);

    let start_time = Instant::now();
    let _ord = optimal_sort::optimal_sort(&m);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("\nOptimal sort elapsed time: {} ms", milliseconds);
}
