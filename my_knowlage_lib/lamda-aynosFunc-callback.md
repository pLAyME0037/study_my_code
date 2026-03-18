While these three terms often hang out in the same neighborhoods, they actually
describe different concepts: syntax, identity, and purpose. It’s a classic "all
squares are rectangles, but not all rectangles are squares" situation.

1. **Anonymous Functions** (The Identity)
   An anonymous function is simply a function that does not have a name. Instead
   of declaring a function for reuse later, you create it right where you need it.

  * Key trait: It’s "nameless."

Example (JavaScript):
```js
function() { console.log("I have no name!"); }
```

2. **Lambda Functions** (The Syntax)
   A lambda is a specific type of anonymous function. The term comes from "Lambda
   Calculus" in mathematics. In most modern programming languages, a lambda is
   just a shorthand, concise syntax for writing an anonymous function.

  * Key trait: Concise, often single-line syntax.

Example (Python):
```py
    lambda x: x + 1
```

3. **Callback Functions** (The Purpose)
   A callback is a function that is passed as an argument to another function,
   with the expectation that it will be executed (called back) later.

  * Key trait: It defines when or how a function is used, not how it is written.
      * The Twist: A callback can be a named function, an anonymous function, or a lambda.

## How They Work Together

```js
// 'handleClick' is a named function used as a callback
button.addEventListener("click", handleClick); 

// This is an anonymous function (and a lambda/arrow function) used as a callback
button.addEventListener("click", () => { console.log("Clicked!"); });
```

In the second example:

It is anonymous (no name).
It is a lambda (uses the => arrow syntax).
It is a callback (it’s passed into addEventListener to be run later).

|Term|Refers to...|Question it answers|
|:---|:---|:---|
|Anonymous|Identity|Does it have a name?|
|Lambda|Syntax|"Is it written in a compact, functional style?"|
|Callback|Role/Usage|Is it being passed into another function to be run later?|
