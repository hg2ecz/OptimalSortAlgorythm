extern crate cc;

fn main() {
    cc::Build::new()
	.file("src/optimal_sort.c")
	.flag("-Ofast")
	.flag("-march=native")
	.include("src")
	.compile("liboptimal_sort.a");
}
