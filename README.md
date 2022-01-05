# MultithreadingTest
### Elements
Class Element and its childrens Element1, Element2, Element3 get/sets such data:
1. Class name (read only)
2. User text (read and write)
3. Counter.  (read and write)
4. Displayed text: class name + user text + counter (read only).
### ElementList
Thread-safe list of elements ElementList, whose elements can be instances of any of the above Element classes.
### Workers
The worker randomly selects the action "Add", "Edit", "Delete".
If the action "Add" is selected, then randomly creates item Element1, Element2 or Element3 and adds it to the ElementList.
If the action "Edit" is selected, then randomly selects an element not occupied by the user or another worker and increments its counter. If there are no elements or no free element, then do nothing.
If the action "Delete" is selected, then randomly selects an element not occupied by the user or another worker and deletes it from the ElementList. If there are no elements or no free element, then do nothing.
During each action, a worker sleeps for n ms (default is 1000).
As many workers are created as specified in the interface (default is 5).