# Contribute to Hallpass Hero!

 ## Making pull requests:
1. Fork the repo and create your branch from `dev`.
2. Ensure you have read the `README.md` and have all dependencies install on your machine.
3. When naming your branch please follow these guidelines:
  - Prefix all branch names with the language that majority of the work for th PR is written in. Examples: `c/`, `python/` .
  - Following the `/` add no more than <b>5</b> words separated by `-` explaining what is being worked on in the branch. <i>Examples:</i>  ```c/fix-ui-crash-bug``` or ```python/update-data-loader-script```
  - If the branch has an associated issue, add the issue number to the end of the branch name. <i>Examples:</i>
 ```c/fix-ui-crash-bug-212``` or ```python/update-data-loader-script-398```
 ***NOTE***  If there is <b>NOT</b> an associated issue, a number at the end of your branch name is <b>NOT</b> required.
4. When submitting your PR, request to merge it the `dev` branch.


## Making an issue:

<p><b>Features Requests</b></p>

1. If the issue is a feature request prefix the issue with `Feature:` Following the colon add a brief description about the feature you are requesting. <i>Example:</i>
```Feature: Add more settings to settings menu```

2. Add a description that gives further context and justification about the feature you are requesting. ***NOTE*** If you have any technologies, working examples, information sources, etc that could help implement this feature please add them to to the comment. <i>Example:</i>
```I think users could benefit from having more settings to customize their experience and UI.  I think it would be cool to add a feature that allows users to change the font size of the text in while in the CLI. I think it would be best to implement this feature in the C code.```

<p><b>Bug Reporting</b></p>

1. If the issue pertains to a bug that you or someone else has found prefix the issue with `Bug:` Following the colon add a brief description of the bug. <i>Example:</i>
```Bug: Program crashes trying to access the settings menu.```

2. Add a comment explaining where the bug was found. This can be done in text or a screenshot. Include if you have been able to recreate the bug and the steps taken to do so. ***NOTE*** If you have have guidance or thoughts on a possible course of action to fix the bug add it to the comment as well. <i>Example:</i>
```I found this bug when when trying to add multiple students to through the bulk data loader. I took the following steps to recreate the bug:```
`1. Open the program`
`2. Access the bulk data loader.`
`3. Add multiple students.`
`4. Program crashes.` 
`I think the bug is being caused by the program not being able to handle the amount of data being added`