Library for Remote Monitoring of Application Components (`Como`).

This library is intended to provide a simple and convenient interface for remote
monitoring of application components in real time.

# About

`Como` is a C++ library that provides an opportunity to applications using it to send out information
on the sources of data within the application, that is, `Como` provides remote monitoring of applications.
Library name is an abbreviation of two English words: components observation and components monitoring,
this is like someone like that.

The main idea of the library is that the application creates a `Como::ServerSocket` to manage client
connections and send information to customers about the state of data and a number of objects of class
`Como::Source`. The application simply changes the value of the object `Como::Source` at the right moment
and `Como::ServerSocket` sends a message to all connected clients.

The data source can be dynamic. That is appear and disappear at the time of construction/destruction
of a `Como::Source`. The application need not worry about manually sending messages. All the work is
done automatically. That is, when an object of `Como::Source` registered there is sending of
registration of data source messages, when the value of the data source is changed there is
sending messages about the value changes of the data source, and the destruction
of the object is sending deregistration of data source messages.

Class objects `Como::Source` may be in a different threads and not have to worry about synchronizing
class `Como::ServerSocket`. `Como::ServerSocket` class is thread-safe.

In fact it is all you need to know to start using the library `Como`.

There is an application for viewing `Como` sources [Globe](https://github.com/igormironchik/globe-como)
