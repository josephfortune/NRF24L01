----------------------------------------
AVR Library for the NRF24L01 Transceiver
      Written by Joseph Fortune
----------------------------------------

I. Liscense 

	This code is distributed under the Beerware liscense. Any individual is free to do as they please with
	this source under the condition that if they ever meet the author and they like the code, then they 
	buy him a beer. 

II. General Usage

	No device has ever caused me as much frustration as the NRF24L01 transceiver. It is HIGHLY sensitive to
	to its power source, so much that I could not get it to work with a variable DC power supply, even with 
	multiple capacitors inline. If you are powering the NRF24L01 with anything other than the Arduino source,
	make sure it has a nice clean source.

	The included example code demonstrates the basic functionality of sending and receiving, including an
	interrupt handler. If the user intends to deviate much from this code, I would recommend reviewing the
	NRF's datasheet to get a basic understanding its functionality. 
 
