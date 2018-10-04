pub fn merge_sort_recursive(a: &mut Vec<i32>, b: usize, e: usize) {
    if b < e {
        let m = (b+e)/2;
        merge_sort_recursive(a, b, m);
        merge_sort_recursive(a, m+1, e);
        merge(a, b, m, e);
    }
}

fn merge(a: &mut Vec<i32>, b: usize, m:usize, e:usize) {
    let mut left = a[b..m+1].to_vec();
    let mut right = a[m+1..e+1].to_vec();
    left.reverse();
    right.reverse();
    //println!("size of right: {}", right.len());
    for k in b..e + 1 {
        if left.is_empty() {
            a[k] = right.pop().unwrap();
            continue;
        }
        if right.is_empty() {
            a[k] = left.pop().unwrap();
            continue;
        }
        if right.last() < left.last() {
            a[k] = right.pop().unwrap();
        }
        else {
            a[k] = left.pop().unwrap();
        }
    }
}
