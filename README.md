# dbc-main
DB Currency Core Library

This contains the base library, that takes care of:
- FullNode / LightNode operation
  - Network connection, auto discovery
- Mining node operation
- Certification node operation
- Direct SQL interface

It uses:
- SQLite - As the core technology inside
- crytopp - As the tools for hashing and mining
- A lot of C++11 code to implement everything on top of those

This contains no code for UI, or UI related things.
It is the UI that performs SQL querys to the core if it wants to search some specific data inside the DB.
