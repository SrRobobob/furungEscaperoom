import threading
import time

class EventBus:
    def __init__(self):
        self.listeners = {}

    def subscribe(self, event_type, listener):
        if event_type not in self.listeners:
            self.listeners[event_type] = [listener]
        else:
            self.listeners[event_type].append(listener)

    def unsubscribe(self, event_type, listener):
        if event_type in self.listeners:
            self.listeners[event_type].remove(listener)
            if not self.listeners[event_type]:  # Remove event_type if no listeners left
                del self.listeners[event_type]

    def emit(self, event_type, data):
        if event_type in self.listeners:
            for listener in self.listeners[event_type]:
                listener(data)

    def emitDelayed(self, event_type, data, delay):
        def delayedEmit():
            time.sleep(delay)
            self.emit(event_type, data)
        threading.Thread(target=delayedEmit).start()

# Example Usage

def handle_event(data):
    print(f"Event received with data: {data}")

# Create an EventBus instance
bus = EventBus()

# Subscribe to an event
bus.subscribe('my_event', handle_event)

# Emit an event immediately
bus.emit('my_event', 'Immediate Hello World!')

# Emit an event after a delay of 5 seconds
bus.emitDelayed('my_event', 'Delayed Hello World!', 5)
