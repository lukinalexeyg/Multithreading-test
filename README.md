# MultithreadingTest
### Elements
Class Element and its childrens Element1, Element2, Element3 get/sets such data:
- Class name (read only)
- User text (read and write)
- Counter (read and write)
- Displayed text: class name + user text + counter (read only).
### ElementList
ElementList is a thread-safe list of elements, whose elements can be instances of any of the above Element classes.
### Workers
- A worker randomly selects the action "Add", "Edit", "Delete".
- If the action "Add" is selected, then randomly creates item Element1, Element2 or Element3 and adds it to the ElementList.
- If the action "Edit" is selected, then randomly selects an element not occupied by the user or another worker and increments its counter. If there are no elements or no free element, then do nothing.
- If the action "Delete" is selected, then randomly selects an element not occupied by the user or another worker and deletes it from the ElementList. If there are no elements or no free element, then do nothing.
- During each action, a worker sleeps for n ms (default is 1000).
- As many workers are created as specified in the interface (default is 5).