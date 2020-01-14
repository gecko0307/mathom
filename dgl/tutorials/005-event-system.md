Event System
============
DGL includes an easy to use event system that can be seen as an object-oriented wrapper on top of SDL events. For those who is unfamiliar with this concept: event system is an abstractized way for a program to perform actions under certain conditions or react to real time changes of some state. For example, this may be reacting on keyboard input, window resize, application exit, etc. In object-oriented event model, you are not required to query state information manually: all you do is just define special handler methods for your objects.

In DGL, event system consists of one event manager any many event listeners. `EventManager` and `EventListener` classes are defined in `dgl.core.event`. Event manager is responsible for querying state information from operating system via SDL and generating events. Event listener is an  entity that reacts to events. Any object can be event listener, if it derives from `EventListener`. This works like follows:

```d
class MyObject: EventListener
{
    this(EventManager emngr)
    {
        super(emngr);
    }
    
    // Key press event handler
    override void onKeyDown(int key)
    {
        // ...
    }
    
    // Probably other event handlers...
}
```

Because `EventManager` doesn't have direct control over its listeners, event reaction procedure (`processEvents`) must be explicitly called by the programmer for each listener. For example, you can store an array of `EventListeners` in your application class and call `processEvents` for them in a loop on every update:

```d
override void onUpdate(double dt)
{
    super.onUpdate(dt);
    foreach(listener; listenersList)
    {
        listener.processEvents();
    }
}
```

This way you can enable/disable event processing for each object individually, which is very handy in game logics programming (think of e.g. pausing or freezing while waiting for input).

Note: `onUpdate` and `onRedraw` are not really event handlers. They are just convenient methods defined for `Application` that you can override. The order in which they are called in the main loop is the following:

```d
eventManager.update(); // generate events
processEvents(); // call Application's own event handlers
onUpdate(); // call other event listeners' event handlers
onRedraw(); // render graphics
```

The following event handlers can be defined: 
* `onKeyDown` - generated when user presses a key (keyboard events are generated only once - continuous check can be done via `EventManager`)
* `onKeyUp` - generated when user releases a key
* `onTextInput` - generated when user types a text character or control symbol on a keyboard. In contrast to `onKeyDown`, this event retrieves international characters based on current keyboard layout. The characters are 32-bit Unicode code points (`dchar`)
* `onMouseButtonDown` - generated when user presses a mouse button (mouse events are generated only once - continuous check can be done via `EventManager`)
* `onMouseButtonUp` - generated when user releases a mouse button
* `onJoystickButtonDown` - generated when user presses a joystick button (joystick events are generated only once - continuous check is currently not supported, but you can implement it yourself)
* `onJoystickButtonUp` - generated when user releases a joystick button
* `onJoystickAxisMotion` - generated when user moves joystick axis (analog controller)
* `onResize` - generated when user changes the size of the application window. If the window is made non-resizable (via `windowResizable` option in `game.conf`), this event is never generated
* `onFocusLoss` - generated when user minimizes application window to tray or changes focus to another window (currently there are no way to distinguish between these two)
* `onFocusGain` - generated when user restores application window from tray or changes focus to it (currently there are no way to distinguish between these two)
* `onQuit` - generated when user closes application window
* `onUserEvent` - generated when user event is spawned (see below)

User events are a powerful way of communication between game objects without using an explicit global state. Any `EventListener` can spawn a user event with `generateUserEvent` method. User events are distinguished by numeric codes.

Example:

```d
enum MY_EVENT_CODE = 10;

class MyObjectA: EventListener
{
    this(EventManager emngr)
    {
        super(emngr);
    }
    
    override void onKeyDown(int key)
    {
        generateUserEvent(MY_EVENT_CODE);
    }
}

class MyObjectB: EventListener
{
    this(EventManager emngr)
    {
        super(emngr);
    }
    
    override void onUserEvent(int code)
    {
        if (code == MY_EVENT_CODE)
        {
            // Process this user event
        }
    }
}
```
