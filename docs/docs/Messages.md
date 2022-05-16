## Creating messages
Some messages just have a string of text, for example, XML or JSON.  Other messages have binary data, such as PCF, where there might be a text field, with its length.

If you have messages with binary data you can use the [struct](https://docs.python.org/3/library/struct.html) capability of Python to convert from a python structure to a blob of data.

You create a structure like

    data = [ 
        ['ChannelName', b'MYCHANNEL', '20s'], 
        ['Version', CMQXC.MQCD_VERSION_6, MQLONG_TYPE], 
        ['ChannelType', CMQC.MQCHT_CLNTCONN, MQLONG_TYPE], 
        ['TransportType', CMQC.MQXPT_TCP, MQLONG_TYPE], 
        ['Desc', b'', '64s'],...

Where the element are

1. The field name ('ChannelName')
2. The value (b'MYCHANNEL')
3. The field type, 20s is a string of length 20, MQLONG_TYPE is i for 64 bit, and l for 32 bit.

You use 
    
      xdata = data.pack()

to create a binary representation of the data.

To decode a message you use
   
     buffer = xdata
     mydata = data.unpack(buffer)

and this will create a dictionary from the buffer using the format specification in data.

You can use pass a format string and data items in one function call.

    packed = struct.pack('i 4s',10,b'Name')
Where the format is "i 4s" and the values are 10, and b'Name'

and pass the formatting string and the blob of data into the function

   unpacked = struct.unpack("i 4s",packed)

where packed is the buffer to be unpacked using the two formats; "i" and "4s".