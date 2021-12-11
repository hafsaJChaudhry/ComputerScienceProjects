Assignment 2
1. Write a function that returns the count of characters and words in a string. The string is provided to the function as an argument.  Submit a .ipynb file via Blackboard. 
2. Write a function that takes a list as the argument and returns a list with each element shifted right by one index. 
For example, [3, 7, 4, 1] becomes [1, 3, 7, 4].  Submit a .ipynb file via Blackboard. 
3. Download data in CSV format from one of the following sites: here or here. Load the data into a Pandas dataframe. Use Pandas to count the number of rows of data and number of columns. Your selection of a CSV should have at least 2 columns and at least 10 rows. Include a link to the data source in the Juptyer notebook you submit.


********************************************************************************************
Assignment 3
Write a Python notebook that generates a file containing the following data:

•Email addresses

•Phone numbers

•Home Address

•Person's name

•Year born. Use realistic values.

•Number of kids. Use realistic values.

•Categorical variable: rent or own?

•Annual income. Optional challenge: Use a non-uniform distribution

•Number of speeding tickets in past year. Optional challenge: Use a non-uniform distribution

User of your notebook should be able to specify how many entities are to be generated.
Generate data in 2 of the following 3 formats: XML, CSV, or JSON. Your choice!
Order of columns in CSV is not relevant.


********************************************************************************************
Assignment 4
Attached Files:
File xml_containing_html.xml.zip (47.042 KB)
A
ttached a compressed XML file. Some of the fields contain HTML. ​

Extract the XML from the .zip file. In Python, use a module to parse the XML (do not write an XML parser)​

Using Python, extract the HTML from the XML. Then use BeautifulSoup4 to parse the HTML 

For each HTML page, report the number of links (URLs with the tag < a href="URL">text) in each HTML page 

Submit a single Jupyter notebook that parses the XML file and produces the count of links per HTML file.​
Advanced students: if you complete the assignment above and are are seeking a challenge, use an alternative method (i.e., regular expressions or Python's find) to validate the count of HTML links per page reported by BeautifulSoup4.
********************************************************************************************
Assignment 5

Part 1: Acquire power data (source) for at least 10 days and not more than 40 days. Load the data into a Jupyter Notebook. Create two bar graphs of the power consumption per hour. One bar graph has 24 bars; one bar graph has 24*(number of days) bars. Submit the .ipynb file containing the analysis and the generated pictures.

Part 2: Simulate a fair die and a biased 6-sided die. The biased die has probabilities {0.15, 0.15, 0.15, 0.15, 0.15, 0.25}. Create a visualization that compares outcomes of multiple rolls of a fair die and this biased die. You can use a single visualization or multiple visualizations to demonstrate the difference in outcomes for the dice. The user of your notebook should be able to alter the number of simulations as an argument to a function. (Help on visualization)
********************************************************************************************
Assignment 6
Attached Files:
File week_05_homework_XLSX_openpyxl.xlsx (21.479 KB)

Attached an Excel spreadsheet that has two worksheets, "main" and "another". Each worksheet has data and visualizations. Worksheet "main" has a "patient id" column; and worksheet "another" has a "p_id" column. The "patient id" values in "main" tab are the same variable as "p_id" in "another".

Submit a notebook that reads the Excel spreadsheet and produces a separate spreadsheet with the following modifications:

1. Use openpyxl to copy patients from "another" to "main"

2. For patients on "another" that don't exist in "main," create new rows in "main"

3. Make no changes to the visualizations that exist in each worksheet.

4. Make no changes to the data on "another".

5. Write your changes to a new .xlsx file (don't overwrite the original)

Some helpful observations:
* "main" worksheet will have three new columns (because those columns exist in the "another" worksheet)
* "main" worksheet will have new rows (one row per patient)
* There will be empty cells in "main" worksheet
* Use a programmatic (rather than manual) approach to identify which patients appear in both worksheets
* Some cells in both worksheets contain formulas. Copy only values from "another" to "main"
