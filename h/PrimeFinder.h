/* 
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>
 * 
 * ==========================================
 * @author Aryan Gupta
 * @project Prime Finder
 * @title Prime Finder Using Multiple Ways
 * @date 2017-02-12 (YYYY-MM-DD)
 * @version 5.3
 * @description Finds prime numbers by multithreading using sections,
 * 				multithreading using single numbers, by single threading,
 * 				also tests if a number is prime, and finds the next 
 *				prime number
 * ==========================================
 */

#ifndef _PRIMEFINDER_H
#define _PRIMEFINDER_H

void helpText();

parseInfo parse(int argc, char* argv[]);

int main(int argc, char* argv[]);

#endif