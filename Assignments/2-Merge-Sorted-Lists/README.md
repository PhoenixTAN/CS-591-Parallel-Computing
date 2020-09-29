# READ ME --- merge two sorted list

Ziqi Tan           U 88387934

Xueyan Xia      U 82450191

## Problem

Consider the problem of merging two sorted arrays of numbers A and B of length m and n, respectively. Design a parallel merging algorithm on a CREW PRAM using (m+n) / log(m+n) processors. What is the time complexity of your algorithm?

 Time complexity is 
$$
O(log^2(m+n))
$$

## Methodology

### Naive

Firstly, we can think about how to do this by using (m+n) processors.

List1: [2, 4, 6, 8, 10]

List2: [1, 2, 3, 4, 5]

List3: [ ]

Every process takes only one element.

For example, process 0 take the first element 2 in list1 of index 0.

Then, perform binary search and find out that 2 should be in the 2nd position (starts from 0) in list2 if we insert 2 into list2.

Then, we put it into list3 (i.e. list3[0+2] = 2).

For one more example, process 7 take the second element 3 in list2 of index 2.

Then 3 should be in the 1st position (starts from 0) in list1 if we insert 3 into list1.

Then, we put it into list3 (i.e. list3[1+2] = 3)

```
list3 = [ ]
forall element in list1 and list2 do in parallel:
	if the element is from list1:
		let's say this element is list1[i]
		find the position j if we want to insert it into list2
		list3[i+j] <- this element
	if the element is from list2:
		let's say this element is list2[j]
		find the position i if we want to insert it into list1
		list3[i+j] <- this element
```

### Using (m+n) / log(m+n) processors

The only thing we need to do is partitioning all m+n elements into (m+n) / log(m+n) parts. Each processor computes only one part where there are log(m+n) numbers. The time complexity for binary search is O(log(m+n)). Therefore, the time complexity is 
$$
O(log(m+n) * log(m+n)).
$$

## Result

```
list1: [0, 2, 4, 6, 10, 13, 18]
list2: [0, 1, 6, 11, 15, 18, 22, 26, 28]
sequential version result: [0, 0, 1, 2, 4, 6, 6, 10, 11, 13, 15, 18, 18, 22, 26, 28]
parallel version result: [0, 0, 1, 2, 4, 6, 6, 10, 11, 13, 15, 18, 18, 22, 26, 28]
True
```

