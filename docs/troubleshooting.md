[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Troubleshooting

With such an interactive program such as #k@, it is expected that one will receive errors in running their simulations from time to time. Here we will discuss some of the error messages and errors you may come across while working with #k@, and how to find a solution to them.

## Error Messages

When trying to run #k@ after configuring the **INFILE.yaml** file, you may be unable to run your network simulation and recieve one of the following error messages:

### Spelling Error

`
Exception occurred while reading 'INFILE.yaml-generated': yaml-cpp: error at line 90, column 17: invalid scalar
terminate called after throwing an instance of 'YAML::InvalidScalar'
  what():  yaml-cpp: error at line 90, column 17: invalid scalar
/home/mobile/hashkat/run.sh: line 80:  4026 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

This error message is due to a spelling error that you may have accidentally made in **INFILE.yaml**, such as spelling *true* or *false* wrong. We received this error message when we spelt *false* as *fAlse* under *use_barabasi*. This error can be simply remedied by fixing the spelling mistake and rerunning #k@.

### Indentation Error



### Incorrect Function Formatting

`
Exception occurred while reading 'INFILE.yaml-generated': yaml-cpp: error at line 124, column 8: 'y_intercept' was not found!
terminate called after throwing an instance of 'YAML::RepresentationException'
  what():  yaml-cpp: error at line 124, column 8: 'y_intercept' was not found!
/home/mobile/hashkat/run.sh: line 80:  4312 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

This error message is due to improper formatting in at least one of the functions present in your **INFILE.yaml* file. When using a constant function for your add rate, follow rate, or tweet rate, you must have a value for this constant rate. When using a linear function, you must have a y-intercept and slope for this increasing or decreasing rate. If these corresponding variables are not present for each particular function, then you will receive an error similar to the one above, which was caused by having a linear add rate with a corresponding value instead of a y-intercept and slope. Simply changing your function configuration so that it coincides with the function you are implementing will fix this error.


### Running Out of Memory



### Implementing *use_barabasi* For the Wrong Follow Model



## Errors

### Twitter Suggest Follow Model Network Not Constructed As Such



### No Retweets Present in Network When They Should Be


## Reporting an Issue

