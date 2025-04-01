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

