# CSC8502 Coursework – 2016/17

Due: 25th November

The coursework builds on the skills you have learned in advanced graphics
programming, to render a real-time interactive scene involving many graphical
effects.

The challenge is to use OpenGL and C++ to create a graphical simulation of an
alien planet surface.

The planet could be of any type – rock, ice, lava, forest, desert, lagoon, etc.

A massive explosion can be triggered in the scene.

The goal of the coursework is to integrate as many graphical effects as you can
into the scene, while keeping the frame-rate sufficiently high to maintain an
interactive experience. The specification is therefore deliberately open-ended.

You will not lose marks for using code from the tutorials, although further
marks will be gained for introducing more complex elements. You will also not
lose marks for using textures/models derived from outside sources for elements
in the scene.

Remember that all such external material must be referenced and attributed
properly.

The coursework consists of two parts. The first part is to develop the standard
features of the scene to a level of high quality. The second part is to integrate
multiple advanced graphical effects into the scene, while maintaining a cinematic
framerate.

## Coursework part A – standard features

This is the list of standard features which must be well implemented and
integrated into the scene.

- [x] The planet surface (not the height-map that is provided in tutorials)
- [x] The environment should be lit with ambient, diffuse and specular
- [ ] Shadows
- [x] Skybox
- [x] Explosion is triggered by pressing a key. It consists of
  - [x] A bright flash which overwhelms the screen
  - [x] Particle effects
- [x] A navigable camera, moving around the 3D environment
- [x] Display the framerate being achieved
- [x] Display the total memory being used

## Coursework part B – advanced features

This is a list of ideas for enhancing the scene graphically.

You may choose features from this list or add your own ideas.

The more complex the feature and the better implemented and integrated, the
higher the mark awarded for that feature.

- [ ] Planet surface extends as far as the eye can see
- [ ] Explosion causes permanent damage to planet
- [ ] Vegetation which grows as the scene runs (i.e. it evolves over time
  programmatically)
- [ ] Space ships, UFOs, strange lights in the sky (lighting is key here not the
  space ship model)
- [x] Weather effects (rain, snow, fog, wind, lightning)
- [x] Volcano or geyser eruption
- [x] A portal effect
- [x] Lens-flare
- [ ] A light that casts shadows appropriately (ie from the light’s position in all
  directions)
- [x] A pool of realistic water which reflects the scene
- Any additional post-processing effect
- Any other advanced graphical effects

## Deliverable items (submitted via NESS)

- Source code (zip file)
- A document containing at least four screen shots and a link to a YouTube video
  of your coursework running
- Demonstrate the program running in the lab to Gary and Richard

## Marking

Marks Available (50)

### Coursework A (25 marks)

Implementation of the standard features gains up to 25 marks.

A "first class" submission consists of these features implemented with
well-structured code, running at 60fps in full screen on the lab PC.

### Coursework B (25 marks)

Advanced rendering techniques (the optional extras above) gain up to 25 marks.

A "first class" submission entails a balanced scene of graphical effects
including at least four advanced graphical features (well implemented and
integrated with the standard features), running at a minimum of 60fps in
full-screen mode.

The code must be clearly formatted.

## Learning Outcomes

- Identify appropriate techniques for rendering graphics in real-time
- Describe graphical representations mathematically
- Realise which advanced techniques are required to achieve realism
- Use advanced techniques associated with lighting to create realism in
  graphical scenarios
- Balance processing and memory requirements of multiple graphical effects at
  cinematic frame rate
