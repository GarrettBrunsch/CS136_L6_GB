**4/24/2025**
*  **feedback.md is for instructor use only. DO NOT change the feedback.md**; make a copy if needed
* class definition style & standards:  first, member vars  none per line and do not forget to initialize them;  second functions: separate group of functions  by a blank line; either matching setter/getter pairs or list all setters back to back and list all getters back to back; start with a default c’tor, followed by  copy c’tor (if any), then other c’tors in the order of increased number of arguments, d’tor- right after c’tor(s), overloaded functions- list them back to back in the order of increased number of arguments;  all accessor/getter functions should be const; all functions with more than one statement should be normal (defined externally, outside of the class and below main()), no inline functions with more than one statement; initialize all member variables to appropriate default values at the time of definition; all member variables must be private; classes must provide a strong exception guarantee; must have default c’tor; implement Rule of Three when necessary
* uses #include \<utility\> -5
* readability, comments; do not state the obvious, do not reiterate/duplicate the code, remove outdated code; comments supposed to help and not create unnecessary clutter; "Code never lies, comments sometimes do." - Ron Jeffries”; what is the purpose of the comments as this(ese) one(s); -2
```text
// Hard coded test files to comply with assign description
const string IDENTICAL_CASE = "in_identical.txt";
```
* Rule of Three implementation
  * copy constructor:
    * a new object ("destination" object) is being created by copying another object ("right" object, source object) of the same class
    * what would you do I f the copy process was not successful? keep in mind that if you terminate the c'tor function, the destination object will not be created; should you create an empty object instead? what would you do with already allocated memory?
  * copy assignment operator -10
    * the "left" object already exists and might have already allocated nodes
*217  when temp  goes out of scope, so will its nodes
    * should you delete all nodes in your left object first and then copy nodes from the "right"  object? if yes, what happens if the copy process is not successful (could not copy all nodes)? If you delete all nodes in your left object before confirming that the copy process succeeded, you lost the original nodes in the left object and did not copy all nodes from the right object. Would you rather have your left object intact than have a bad copy of a right object?
  * destructor
    * do not forget to clean up dynamically allocated memory (DMA);  a function that deallocates all DMA is quite helpful- iyt can be called  in D’tor and other places such as catch(bad_alloc)
    * if you use delete on something that was not dynamically allocated, a compiler might throw an exception; check first, then use delete


**4/1/2025**
* class definition style & standards:  first, member vars and do not forget to initialize them;  second functions: separate group of functions  by a blank line; either matching setter/getter pairs or list all setters back to back and list all getters back to back; start with a default c’tor, followed by  copy c’tor (if any), then other c’tors in the order of increased number of arguments, d’tor- right after c’tor(s), overloaded functions- list them back to back in the order of increased number of arguments;  all accessor/getter functions should be const; all functions with more than one statement should be normal (defined externally), no inline functions with more than one statement; initialize all member variables to appropriate default values at the time of definition; all member variables must be private; classes must provide a strong exception guarantee; must have default c’tor; implement Rule of Three when necessary
* may resubmit no later than 4/13/25
* missing default c’tor(s) -5
* #88 what for? might become stale
* copy c’tor, copy assignment operator, and d’tor were not tested
* Queue class and main() are haphazardly written
* readability: initialize the list makes code harder to read; write assignment statements instead
*  function definitions above main() -10
*  do not use this-> unless you really have to, it reduces readability; -2
* #157 – what about nodes that have already been created?  -20
* #172 –  will terminate the c’tor prematurely =>an object will not be created=> you might have a name/var id for an object but not the object; still need to create the object even if it is an “empty” object; who is catching bad_alloc? -10
* #199 and like who is catching bad_alloc? 
* #205 ~Queue  will  release all nodes from  #182  temp queue =>  all nodes from a copied queue will be released -10
* #215-236 poor use of try/catch – “ if everything goes well” goes into try block
* #326 why? Try/catch is a computational expense; it works like a function call; do not use it unless you really have to; simple if statement should suffice here; if files don't match it's not an exception;

