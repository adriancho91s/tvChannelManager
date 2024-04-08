# TV Channel Manager Program

This program allows you to manage TV channels and their programs. You can add channels, query channel information, delete channels, delete all tv programs of each channel, show the channel guide, and exit the program.

## Features

- **Add a Channel**: Add a new channel to the TV guide.
- **Add a program**: Add a new program to each channel with name, time slot and duration.
- **Query Channel Information**: Search for channel information by number or name.
- **Delete a Channel**: Remove a channel from the TV guide.
- **Delete all tv programs from a channel**: Remove all tv shows from a channel.
- **Show the Channels Guide**: Visualize the list of channels with navigation options.
- **Exit the Program**: Quit the program.

## Usage

1. **Adding a Channel**: Choose option 1 from the menu and enter the channel number and name.
2. **Querying Channel Information**: Choose option 3 from the menu and search by number or name.
3. **Deleting a Channel**: Choose option 4 from the menu and enter the channel number or name.
4. **Showing the Channels Guide**: Choose option 5 from the menu to navigate through channels.
5. **Exiting the Program**: Choose option 6 from the menu to exit.

## Development

This program is written in C and utilizes doubly linked lists for channel management. It's designed for simplicity and ease of use.

## How to Run

This project uses CMake for building. To build and run the project, use the following commands:

```bash
mkdir build
cd build
cmake ..
make
./doublyLinkedDb
```

## Contributing

Contributions are welcome. Feel free to suggest improvements or report bugs.

## License

Licensed under the MIT License. See [LICENSE](LICENSE) for details.
