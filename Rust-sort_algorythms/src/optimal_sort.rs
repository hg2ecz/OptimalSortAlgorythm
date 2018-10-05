pub type Sorttype = i32;

pub fn optimal_sort(m: &Vec<Sorttype>) -> Vec<Sorttype> {
    let mut o1 = m.clone();
    for i in (0..m.len()-1).step_by(2) {
	if o1[i] > o1[i+1] {
	    o1.swap(i, i+1);
	}
    }

    let mut diff = 2;
    while diff < m.len() {
	let mut o2: Vec<Sorttype> = Vec::new();
	for i in (0..m.len()).step_by(2*diff) {
	    let mut j=i;
	    let mut k=i+diff;
	    while j < i+diff && k < i+2*diff {
		if j < i+diff {
		    if o1[j] <= o1[k] {
			o2.push(o1[j]);
			j+=1;
		    }
		}
		if k < i+2*diff {
		    if o1[j] >= o1[k] {
			o2.push(o1[k]);
			k+=1;
		    }
		}
	    }
	    for jj in j..i+diff {
		o2.push(o1[jj]);
	    }
	    for kk in k..i+2*diff {
		o2.push(o1[kk]);
	    }
	}
	diff <<= 1;
	o1 = o2;
    }
    return o1;
}
