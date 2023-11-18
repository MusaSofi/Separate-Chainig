# Separate-Chainig

ADS_set: A Container for Key-Identified Elements

ADS_set is a container that stores elements identified by a key. The key is both the type of the key and, at the same time, the type of the element. This means that the entire stored element serves as the key. ADS_set provides a simplification and generalization of the STL types std::set and std::unordered_set.

In this container, each key can appear at most once (set property). Therefore, there cannot be two elements with the same key in the container. Unless explicitly specified otherwise for individual methods/functions, attempting to insert values that are already present is silently ignored. The same applies when attempting to delete non-existing values.

In the container, copies of the elements passed to the corresponding methods are always stored. The container is subsequently responsible for the proper destruction of these elements. The data structures must, aside from system limitations, in principle, be able to accommodate any number of values. For data structures with a fixed size (e.g., static hash methods), if there is no space left or the predefined maximum occupancy is exceeded, the data structure should be enlarged accordingly. For static hash methods, this involves creating a new (larger) table and rehashing values. The data structure is intended to "grow" with the number of stored values.

It's important to note that the elements stored in the container should not be modified (e.g., through iterators) as this can disrupt the consistency of the container.