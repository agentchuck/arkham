# arkham
Forbidden researches of the old ones

# Workitems
- Find phrases of power

# Desired Algorithm
1. Determine the possible endpoints given starting location of piece
 - Watch for failure/replicated states
2. For each endpoint, determine optimal score. Try to use power phrases.
3. For each endpoint, restart from 1.
4. Coordinate over multiple units to maximize line clears over multiple turns.
   (Yeah... ok...)

# Notes
  redblobgames.com - Hex coordinate information
  - http://www.redblobgames.com/grids/hexagons/#coordinates
  - We're using odd-r
  - When rotating, do we need to convert odd/even-r?...

# Phrases of power
- Notes...
  Seem to be lovecraftian, probably also related to FP.
  Need final punctuation? Would satisfy 51 characters with chant...
  Can they be nested? Likely not.
  18 phrases between 3 and 51 characters in length.
  Look in the sample problems.
  Need non-trivial sample problems to test rotations.

- Known:
ei!
 - E, SW, W -> Translates to SW.
ia! ia!
 - SW, SW, W, SE, SW, SW, W
yuggoth - From map 7
 - E, CCW, SW, SW, SE, CCW, SW

- Possible:
ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn - Famous HPL reference

bigboote - From twitter

Eris - Tenth planet

- Wrong:
davar - From postmortem 
 - CW, SW, CW, SW, CW
 - W, CW, SW, CW, SW, CW, W (with quotes)

cthulhu - Hint in spec
 - E, CCW, SW, CCW, SE, SW, CCW

R'lyeh - From map 5
 - CW, W, SE, E, E, SW

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

