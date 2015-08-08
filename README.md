# arkham
Forbidden researches of the old ones

# Workitems
- Find phrases of power

# Desired Algorithm
1. Determine the possible endpoints given starting location of piece
 - Watch for failure/replicated states
2. For each endpoint, determine optimal score. Try to use power phrases.
3. For each endpoint, restart from 1.

# Phrases of power
- Notes...
  Seem to be lovecraftian, probably also related to FP.
  Need final punctuation? Would satisfy 51 characters with chant...
  Can they be nested? Liekly not.
  18 phrases between 3 and 51 characters in length.
  Look in the sample problems.

- Known:
ei!  E, SW, W -> Translates to SW.

- Possible:
davar - From postmortem
 - CW, SW, CW, SW, CW
 - W, CW, SW, CW, SW, CW, W (with quotes)
cthulhu - Hint in spec
 - E, CCW, SW, CCW, SE, SW, CCW
Ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn - Famous HPL reference

yuggoth - From map 7
 - E, CCW, SW, SW, SE, CCW, SW


# Output format:
[ { "problemId": number   /* The `id` of the game configuration */
  , "seed":      number   /* The seed for the particular game */
  , "tag":       string   /* A tag for this solution. */
  , "solution":  Commands
  }
]

# Submission:
export API_TOKEN=Td/Cz+QybNRt/9poXn2R2W2WOLB9nYXtwwUlK+N5Erg=
export TEAM_ID=152

curl --user :$API_TOKEN -X POST -H "Content-Type: application/json" \
        -d $OUTPUT \
        https://davar.icfpcontest.org/teams/$TEAM_ID/solutions

