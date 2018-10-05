extern crate gcc;

fn main() {
    gcc::Build::new()
	.file("src/optimal_sort.c")
	.flag("-Ofast")
	.flag("-march=native")
	.include("src")
	.compile("liboptimal_sort.a");
}
