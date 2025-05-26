#!/usr/bin/env node

const fs = require('fs');

// Function to analyze the log file
function analyzeLogFile(filePath) {
  try {
    // Read the log file
    const logContent = fs.readFileSync(filePath, 'utf8');
    
    // Split the log into lines
    const logLines = logContent.split('\n');
    
    // Initialize counters
    let totalControlMessages = 0;
    let rplControlMessages = 0;
    let disMessages = 0;
    let dioMessages = 0;
    let daoMessages = 0;
    
    // Process each line
    logLines.forEach(line => {
      // Skip empty lines
      if (!line.trim()) return;
      
      // Count all control messages (any line with INFO)
      if (line.includes('[INFO:')) {
        totalControlMessages++;
      }
      
      // Count RPL control messages
      if (line.includes('[INFO: RPL')) {
        rplControlMessages++;
        
        // Count specific RPL message types
        if (line.includes('DIS')) {
          disMessages++;
        } else if (line.includes('DIO')) {
          dioMessages++;
        } else if (line.includes('DAO')) {
          daoMessages++;
        }
      }
    });
    
    // Calculate percentages
    const disPercentage = rplControlMessages > 0 
      ? ((disMessages / rplControlMessages) * 100).toFixed(2) 
      : 0;
    
    const dioPercentage = rplControlMessages > 0 
      ? ((dioMessages / rplControlMessages) * 100).toFixed(2) 
      : 0;
    
    const daoPercentage = rplControlMessages > 0 
      ? ((daoMessages / rplControlMessages) * 100).toFixed(2) 
      : 0;
    
    // Print results
    console.log('Log Analysis Results:');
    console.log('-------------------');
    console.log(`Total control messages: ${totalControlMessages}`);
    console.log(`RPL control messages: ${rplControlMessages}`);
    console.log(`  - DIS messages: ${disMessages} (${disPercentage}%)`);
    console.log(`  - DIO messages: ${dioMessages} (${dioPercentage}%)`);
    console.log(`  - DAO messages: ${daoMessages} (${daoPercentage}%)`);
    
    // Provide analysis
    if (disMessages === 0) {
      console.log('\nAnalysis:');
      console.log('No DIS messages were found in the log. This could mean:');
      console.log('1. The attack was not active during the log capture period');
      console.log('2. The log capture period was too short');
      console.log('3. The malicious node was not properly configured or running');
    } else {
      console.log('\nAnalysis:');
      console.log(`The log shows ${disMessages} DIS messages, which is ${disPercentage}% of all RPL control messages.`);
      if (disPercentage > 10) {
        console.log('This indicates a potential DIS flooding attack, as DIS messages should be relatively rare in normal operation.');
      } else {
        console.log('This is within normal expected ranges for DIS messages.');
      }
    }
    
  } catch (error) {
    console.error(`Error analyzing log file: ${error.message}`);
  }
}

// Check if file path is provided as command line argument
const filePath = process.argv[2] || 'loglistener.txt';
console.log(`Analyzing log file: ${filePath}`);
analyzeLogFile(filePath); 