//
//  Shader.fsh
//  c4
//
//  Created by jrk on 8/2/11.
//  Copyright 2011 flux forge. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
