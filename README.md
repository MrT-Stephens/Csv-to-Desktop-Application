
<h1 align="center">
  <img src="https://github.com/MrT-Stephens/Csv-to-Application/blob/master/Images/CSV_to_Logo.png" alt="drawing" width="200"/>
  <br>
  Csv to... v0.3.1
  <br>
</h1>

<h4 align="center">A desktop application which converts CSV (Comma Separated Values) to many other types!</h4>

<p align="center">
  <a href="#key-features">Key Features</a> ▪︎
  <a href="#installation">Installation</a> ▪︎
  <a href="#how-to-use">How To Use</a> ▪︎
  <a href="#future-improvements">Future Improvements</a> ▪︎
  <a href="#libraries-used">Libraries Used</a>
</p>

<p align="center">
  <img src="https://github.com/MrT-Stephens/Csv-to-Application/blob/master/Images/CSV_to_App_Image.png" alt="drawing" align="center"/>
</p>

## Key Features
* Takes CSV files which include a header in the format:
```csv
column1,column2,column3,column4
item1,item2,item3,item4
item5,item6,item7,item8
```
* The CSV files can include double-quoted strings as shown below:
```csv
column1,"column2",column3,column4
item1,"item2, item2",item3,"it"e"m4"
item5,item6,item7,item8
```
> **Note**
> Every double quote MUST have an ending double quote, does support double quotes within double quotes aswell.

* Converts the CSV data to:
   - CSV with different delimiters.
   - SQL insert statements.
   - Ascii tables.
   - Markdown tables.
   - XML.
   - HTML tables.
   - LaTex tables.
   - JSON objects, arrays, etc.
   - PHP arrays.
   - Ruby arrays.
   - Multi-line data.
   - YAML.
   - BBcode.
   - XLSX.
   - Many more in the future!
* Ability to change the format of the inserted data (click the advanced view button):
   - Uppercase all the data.
   - Lowercase all the data.
   - Capitalise all the data.
   - Remove whitespace from all the data.
   - Transpose all the data.
* If you don't like the result of the changed data you can use the Undo and Redo buttons to undo or redo any changes made to the data.
* By clicking on the column names in the list view you can sort by that column into ascending order by clicking once or descending order by clicking twice.
* Ability to copy the generated data to the clipboard or if you prefer you can download the generated data.
* Ability to edit the data using the edit data dialog by clicking on the '**Edit Data**' button.

## Installation
* To install the application, please download the [latest release](https://github.com/MrT-Stephens/Csv_to-Desktop_Application/releases).
  Then extract the archive and start the '.exe' and it should all just work :).

## How To Use
* If you would like some information on how to use the application please view the '[HowToUse.md](https://github.com/MrT-Stephens/Csv_to-Desktop_Application/blob/master/HowToUse.md)' file.

## Future Improvements
* Currently only supports Windows, but in future hoping to support Mac OS, and Linux.
* Add the ability to generate random data within the application.

## Libraries Used
* [wxWidgets](https://www.wxwidgets.org/) used for the UI components of the application.
* [libxlsxwriter](https://github.com/jmcnamara/libxlsxwriter) used for creating an outputting xlsx files.
