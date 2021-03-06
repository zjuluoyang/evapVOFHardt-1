{
    word alphaScheme("div(phi,alpha)");
    word alpharScheme("div(phirb,alpha)");

    surfaceScalarField phic = mag(phi/mesh.magSf());
    phic = min(interface.cAlpha()*phic, max(phic));
    //surfaceScalarField phir = phic*interface.nHatf();
	surfaceVectorField gradAlphaSf = fvc::interpolate(fvc::grad(alphaS));
	surfaceVectorField nHatf = gradAlphaSf / (mag(gradAlphaSf) + interface.deltaN());
	surfaceScalarField phir = phic*(nHatf&mesh.Sf());

    for (int aCorr=0; aCorr<nAlphaCorr; aCorr++)
    {
        surfaceScalarField phiAlpha = 
            fvc::flux
            (
                phi,
                alpha1,
                alphaScheme
            )
          + fvc::flux
            (
                -fvc::flux(-phir, scalar(1) - alpha1, alpharScheme),
                alpha1,
                alpharScheme
            );
    MULES::explicitSolve
    (
        geometricOneField(),
        alpha1,
        phi,
        phiAlpha,
        alphaSourceSp,
        alphaSourceSu,
        oneField(),
        zeroField()
    );

        rhoPhi = phiAlpha*(rho1 - rho2) + phi*rho2;
    }

    Info<< "Liquid phase volume fraction = "
        << alpha1.weightedAverage(mesh.V()).value()
        << "  Min(alpha1) = " << min(alpha1).value()
        << "  Max(alpha1) = " << max(alpha1).value()
        << endl;
}
