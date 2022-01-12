# MultithreadingTest
### Elements
Class Element and its inheritors Element1, Element2, Element3 get/set such data:
- Class name (read only)
- User text (read and write)
- Counter (read and write)
- Displayed text: class name + user text + counter (read only).
### ElementList
ElementList is a thread-safe list of elements, whose elements can be instances of any of the above Element classes.
### Workers
- A worker randomly selects the action "Add", "Edit", "Delete".
- If the action "Add" is selected, then it randomly creates item Element1, Element2 or Element3 and adds the element to the ElementList.
- If the action "Edit" is selected, then it randomly selects an element not occupied by the user or another worker and increments the element's counter. If there are no elements or no free element, then do nothing.
- If the action "Delete" is selected, then it randomly selects an element not occupied by the user or another worker and deletes the element from the ElementList. If there are no elements or no free element, then do nothing.
- During each action, a worker sleeps for n ms (default is 1000).
- There are created as many workers as specified in the interface (default is 5).