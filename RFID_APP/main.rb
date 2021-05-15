require 'serialport'

ser = SerialPort.new("COM14", 9600, 8, 1, SerialPort::NONE)

puts ser.readlines