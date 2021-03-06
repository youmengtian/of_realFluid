/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    Foam::mixtureRedlichKwong

Description
    Redlich Kwong equation of state for multiple component mixtures using the van der Waals mixing rule

    Mixing Rule --> see Paper

    Title: Van der waals mixing rules for cublic equations of state. Applications for supercritical fluids extraction modelling
    Authors: T.Y. Kwak and G.A. Mansoori
    Journal: Chemical Engineering Science, Vol 41, No. 5, pp. 1303-1309, 1986

SourceFiles
    mixtureRedlichKwongI.H
    mixtureRedlichKwong.C

Author
Christian Lucas
Institut für Thermodynamik
Technische Universität Braunschweig 
Germany

\*---------------------------------------------------------------------------*/

#ifndef mixtureRedlichKwong_H
#define mixtureRedlichKwong_H

#include "specie.H"
#include "autoPtr.H"
#include "redlichKwong.H"
#include "scalarList.H"
#include "DynamicList.H"
#include "label.H"
#include "List.H"
#include "dictionary.H"
#include "scalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class mixtureRedlichKwong Declaration
\*---------------------------------------------------------------------------*/

class mixtureRedlichKwong
:
    public redlichKwong
{
    // Private data 
    
    // Private functions

    //CL: function updates the model coefficients (a,b) of the mixture
    //CL: this is the function with the mixing rule 
    inline void updateModelCoefficients() const; 

protected:
    
    //CL: save the concentrations of each component of the mixture
    //CL: needs to be multiplied by this->W() to get the molar fractions
    mutable DynamicList<scalar> weigths;
    
    //CL: saves a pointer to the pure component classes of the mixture
    mutable DynamicList<mixtureRedlichKwong*> mixtureComponents;
   
    //CL: counts the number of components
    mutable label numOfComp;


    //CL: Variables used in real gas mixture correction

        //CL: If true, the real gas mixture correction is used
        mutable bool  realMixtureCorr_;

        //CL: number of mixture components, needed to calculate the mixture correction factors needed
        //CL: do not mistake this variable with numOfComp,
        //CL: numOfComp is a counter to counts the number of components while the mixture is constructed
        mutable label nCom_;

        //CL: stores real mixture correction coefficients
        mutable DynamicList<scalar> realMixtureCorrCoef_;

public:

    // Constructors

        //- Construct from components
        //CL: needed for operator*
        inline mixtureRedlichKwong
        (
            const redlichKwong& rK,
            label numOfComp,
            DynamicList<scalar> weigths,
            DynamicList<mixtureRedlichKwong*> mixtureComponents,
            scalar a, 
            scalar b,
            scalar rhostd,
            scalar rhoMin,
            scalar rhoMax
        );

        //- Construct from components
        //CL: needed for operator+
        inline mixtureRedlichKwong
        (
            const redlichKwong& rK,
            label numOfComp,
            DynamicList<scalar> weigths,
            DynamicList<mixtureRedlichKwong*> mixtureComponents,
            scalar rhoMin,
            scalar rhoMax
        );

        //- Construct from Istream
        mixtureRedlichKwong(Istream&);

        //- Construct from dictionary
        mixtureRedlichKwong(const dictionary& dict);

        //- Construct as named copy
        inline mixtureRedlichKwong(const word& name, const mixtureRedlichKwong&);

        //- Construct and return a clone
        inline autoPtr<mixtureRedlichKwong> clone() const;

        // Selector from Istream
        inline static autoPtr<mixtureRedlichKwong> New(Istream& is);

        // I-O

            //- Write to Ostream
            void write(Ostream& os) const;


  // Member operators

        inline void operator+=(const mixtureRedlichKwong&);

  // Friend operators

        inline friend mixtureRedlichKwong operator+
        (
            const mixtureRedlichKwong&,
            const mixtureRedlichKwong&
        );

        inline friend mixtureRedlichKwong operator*
        (
            const scalar s,
            const mixtureRedlichKwong&
        );


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const mixtureRedlichKwong&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "mixtureRedlichKwongI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
